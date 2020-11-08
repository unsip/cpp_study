#include "ConnectionManager.hpp"
#include "SignalMask.hpp"
#include "TSQueue.hpp"

#include <csignal>
#include <chrono>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <iostream>
#include <atomic>
#include <cassert>

static std::atomic<bool> IS_DONE = false;
static_assert(ATOMIC_BOOL_LOCK_FREE, "Could not guarantee safe signal handling!");

void sig_handler(int)
{
    ConnectionManager::instance().stop();
    IS_DONE = true;
}

// TODO: Create ThreadPool abstraction to manage finished threads.
void spam_timestamp(
        ConnectionManager::Connection conn
      , phase::threading::TSQueue<std::thread::id, std::thread>& workers
      , phase::threading::TSQueue<std::thread::id, std::thread>& finished)
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
    using namespace phase::signals;
    using namespace phase::threading;

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
            catch (const phase::network::SocketClosedError& ex)
            {}
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

