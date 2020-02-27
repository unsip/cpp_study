#include "TcpSocket.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>
#include <iostream>

int main()
{
    TCPSocket sock;
    sock.bind("127.0.0.1", 9999);
    sock.listen();
    auto conn = sock.accept();

    while (true)
    {
        using namespace std::chrono;

        system_clock::time_point tp = system_clock::now();
        std::time_t t = system_clock::to_time_t(tp);
        std::stringstream ss;

        ss << std::ctime(&t);
        auto s(ss.str());
        s.resize(s.size() - 1);
        std::cout << s;
        std::cout.flush();
        conn.send(s.c_str(), s.size() + 1);
        std::this_thread::sleep_for(seconds(2));
    }
}
