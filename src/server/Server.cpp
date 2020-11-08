#include "TcpSocket.hpp"
#include "utils/TSQueue.hpp"
#include "utils/SignalMask.hpp"

#include <csignal>
#include <chrono>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <deque>
#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <atomic>
#include <cassert>

static std::atomic<bool> IS_DONE = false;
static_assert(ATOMIC_BOOL_LOCK_FREE, "Could not guarantee safe signal handling!");


class ConnectionManager
{
private:
    std::optional<TCPSocket> m_acceptor;
    std::mutex m_mut;
    // Note: List iterators (captured by Connection objects) won't be invalidated upon modification.
    std::list<TCPSocket> m_connections;
    ConnectionManager() = default;

public:
    // Note: All Connection's should be destroyed before ConnectionManager.
    // It's true since ConnectionManager is a singletone with static placement.
    // Be careful not to create static Connection class.
    class Connection
    {
    private:
        std::optional<std::list<TCPSocket>::iterator> m_sock_it;
    public:
        Connection(std::list<TCPSocket>::iterator it)
            : m_sock_it{it}
        {}

        Connection(const Connection&) = delete;
        Connection& operator =(const Connection&) = delete;
        Connection(Connection&& rhv)
            : m_sock_it{std::move(rhv.m_sock_it)}
        {
            rhv.m_sock_it.reset();
        }

        Connection& operator =(Connection&& rhv)
        {
            if (this != &rhv)
            {
                m_sock_it = std::move(rhv.m_sock_it);
                rhv.m_sock_it.reset();
            }

            return *this;
        }

        void send(const char* buf, std::size_t len)
        {
            (*m_sock_it)->send(buf, len);
        }

        std::size_t receive(char* buf, std::size_t len)
        {
            if (buf == nullptr)
                throw std::runtime_error("Invalid buffer value: nullptr");

            return (*m_sock_it)->receive(buf, len);
        }

        ~Connection()
        {
            if (m_sock_it)
            {
                std::lock_guard guard(ConnectionManager::instance().m_mut);
                ConnectionManager::instance().m_connections.erase(*m_sock_it);
            }
        }
    };


    static ConnectionManager& instance()
    {
        static ConnectionManager c;
        return c;
    }
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator =(const ConnectionManager&) = delete;

    void start(const std::string& host, unsigned short port)
    {
        m_acceptor.emplace();
        m_acceptor->bind(host, port);
        m_acceptor->listen();
    }

    Connection accept()
    {
        auto sock = m_acceptor->accept();
        std::lock_guard guard(m_mut);
        m_connections.emplace_front(std::move(sock));
        return Connection {m_connections.begin()};
    }

    void stop()
    {
        m_acceptor.reset();
    }

    void close()
    {
        std::lock_guard guard(m_mut);
        for (auto& sock : m_connections)
            sock.close();
    }

    ~ConnectionManager() { stop(); }
};

void sig_handler(int)
{
    ConnectionManager::instance().stop();
    IS_DONE = true;
}

// TODO: Create ThreadPool abstraction to manage finished threads.
// TODO: Move this_thread from workers to finished (using iterator, see TSQueue.hpp).
void spam_timestamp(
        ConnectionManager::Connection conn
      , TSQueue<std::thread::id, std::thread>& workers
      , TSQueue<std::thread::id, std::thread>& finished)
{
    for (unsigned i = 0; i < 5; ++i)
    {
        using namespace std::chrono;

        system_clock::time_point tp = system_clock::now();
        std::time_t t = system_clock::to_time_t(tp);
        std::stringstream ss;

        ss << std::ctime(&t);
        auto s(ss.str()); s.resize(s.size() - 1);
        std::cout << s;
        std::cout.flush();
        conn.send(s.c_str(), s.size() + 1);
        std::this_thread::sleep_for(seconds(2));
    }


    auto wl = workers.lock();
    auto fl = finished.lock();
    auto it = wl.find(std::this_thread::get_id());
    assert(it != wl.cend());
    fl.emplace(std::move(*it));
    // Does erase throw?
    [&wl, &it]() noexcept {wl.erase(it);}();
}

int main()
{
    ConnectionManager::instance().start("127.0.0.1", 9999);
    TSQueue<std::thread::id, std::thread> workers;
    TSQueue<std::thread::id, std::thread> finished;

    auto accept_conn = [&workers, &finished]
    {
        while (!IS_DONE)
        {
            // Note: We must work with ConnectionManager only in a single thread.
            // That's why we mask all signals, to guarantee the signal handler will be called in this thread context.
            SignalMask mask {Sig::interupt, Sig::terminate};
            mask.unlock();
            try
            {
                auto conn = ConnectionManager::instance().accept();
                mask.lock();
                std::thread t {&spam_timestamp, std::move(conn), std::ref(workers), std::ref(finished)};
                workers.lock().emplace(t.get_id(), std::move(t));
            }
            catch (const SocketClosedError& ex)
            {
                std::cout << ex.what() << std::endl;
            }
        }
    };

    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);
    SignalMask mask {Sig::interupt, Sig::terminate};
    std::lock_guard<SignalMask> sig_guard(mask);
    std::thread accept_thrd{accept_conn};

    while (!IS_DONE)
    {
        // TODO: Exception safety (RAII adapter "ThreadPool"??)
        std::unordered_map<std::thread::id, std::thread> tmp;
        {
        auto tlock = finished.lock();
        // Looping should handle spurious wake ups.
        tlock.wait_for(
            std::chrono::seconds(1)
          , [&tlock]() { return tlock.empty() || IS_DONE; }
        );
        tlock.swap(tmp);
        }

        for (auto it = tmp.begin(), last = tmp.end(); it != last; it = tmp.erase(it))
            it->second.join();
    }

    accept_thrd.join();
}
