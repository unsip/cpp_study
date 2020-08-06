#include "TcpSocket.hpp"
#include "utils/TSQueue.hpp"

#include <chrono>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <csignal>
#include <atomic>
#include <cassert>

static std::atomic<bool> IS_DONE = false;
static_assert(ATOMIC_BOOL_LOCK_FREE, "Could not guarantee safe signal handling!");

// TODO: Create ThreadPool abstraction to manage finished threads.
void spam_timestamp(TCPSocket conn, TSQueue<std::thread>& workers, TSQueue<std::thread>& finished)
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

    TSQueue<std::thread> workers;
    TSQueue<std::thread> finished;

    auto accept_conn = [&sock, &workers]
    {
        while (!IS_DONE)
        {
            auto conn = sock.accept();
            workers.lock().push(std::thread {&spam_timestamp, std::move(conn), std::ref(finished)});
        }
    };
    std::thread accept_thrd{accept_conn};

    while (!IS_DONE)
    {
        // TODO: Exception safety (RAII adapter "ThreadPool"??)
        std::queue<std::thread> tmp;
        {
        auto tlock = threads_queue.lock();
        // Looping should handle spurious wake ups.
        while (tlock.empty())
            tlock.wait();
        tlock.swap(tmp);
        }
        while (!tmp.empty())
        {
            tmp.front().join();
            tmp.pop();
        }
    }

    accept_thrd.join();
    for (std::pair<const std::thread::id, std::thread>& p : threads)
        p.second.join();
