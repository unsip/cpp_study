#include "TcpSocket.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <csignal>
#include <atomic>
#include <cassert>

static std::atomic<bool> IS_DONE = false;


template <typename T>
class TSQueue
{
private:
    struct Impl
    {
        std::mutex m_mut;
        std::condition_variable_any m_cond;
        std::queue<T> m_threads;
    };
    std::shared_ptr<Impl> m_impl;

public:
    class QueueFacet
    {
    private:
        std::shared_ptr<Impl> m_impl;

        friend class TSQueue;
        QueueFacet(std::shared_ptr<Impl> impl)
            : m_impl(impl)
        {
            // Lock mutex
            m_impl->m_mut.lock();
        }

    public:
        ~QueueFacet()
        {
            // Unlock mutex
            if (m_impl)
            {
                m_impl->m_mut.unlock();
                m_impl->m_cond.notify_all();
            }
        }

        QueueFacet(QueueFacet&) = delete;
        QueueFacet& operator= (const QueueFacet&) = delete;

        // shared_ptr becomes nullptr on move, so there won't be double mutex `unlock`
        QueueFacet(QueueFacet&& q) = default;
        QueueFacet& operator= (QueueFacet&&) = default;

        void wait()
        {
            m_impl->m_cond.wait(m_impl->m_mut);
        }

        // TODO: Support move-semantics (pass by value).
        void push(const T& t)
        {
            m_impl->m_threads.push(t);
        }

        void pop()
        {
            m_impl->m_threads.pop();
        }

        auto front() -> decltype(auto)
        {
            return m_impl->m_threads.front();
        }

        auto size() const -> decltype(auto)
        {
            return m_impl->m_threads.size();
        }

        bool empty() const
        { return m_impl->m_threads.empty();
        }

        void swap(std::queue<T>& q)
        {
            m_impl->m_threads.swap(q);
        }

    };

    TSQueue()
        : m_impl(std::make_shared<Impl>())
    {}

    TSQueue(TSQueue&) = delete;
    TSQueue& operator= (const TSQueue&) = delete;

    TSQueue(TSQueue&& t) = default;
    TSQueue& operator= (TSQueue&& rhv) = default;


    QueueFacet lock()
    {
        return QueueFacet {m_impl};
    }
};

void spam_timestamp(TCPSocket conn, TSQueue<std::thread::id>& pqueue)
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

    pqueue.lock().push(std::this_thread::get_id());
}

void sigint_handler(int /*signal*/)
{
    IS_DONE = true;
}

int main()
{
    // Make it quit on Ctrl-C (bug??)
    std::signal(SIGINT, sigint_handler);
    TCPSocket sock;
    sock.bind("127.0.0.1", 9999);
    sock.listen();

    std::mutex tmut;
    std::unordered_map<std::thread::id, std::thread> threads;
    TSQueue<std::thread::id> threads_queue;

    auto accept_conn = [&sock, &threads, &threads_queue, &tmut]
    {
        while (!IS_DONE)
        {
            auto conn = sock.accept();
            std::thread thrd{&spam_timestamp, std::move(conn), std::ref(threads_queue)};
            auto t_id = thrd.get_id();
            std::unique_lock<std::mutex> guard(tmut);
            // TODO: Why tf are we failing here?
            threads.emplace(t_id, std::move(thrd));
        }
    };
    std::thread accept_thrd{accept_conn};

    while (!IS_DONE)
    {
        std::queue<std::thread::id> tmp;
        {
        auto tlock = threads_queue.lock();
        // Looping should handle spurious wake ups.
        while (tlock.empty())
            tlock.wait();
        tlock.swap(tmp);
        }
        while (!tmp.empty())
        {
            {
            std::unique_lock<std::mutex> guard(tmut);
            auto it = threads.find(tmp.front());
            it->second.join();
            assert(it != threads.end());
            threads.erase(it);
            }
            tmp.pop();
        }
    }

    accept_thrd.join();
    for (std::pair<const std::thread::id, std::thread>& p : threads)
        p.second.join();
}
