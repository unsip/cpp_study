#include "TcpSocket.hpp"
#include <iostream>
#include <algorithm>

int main()
{
    TCPSocket sock;
    sock.connect("127.0.0.1", 9999);

    char buf[10];

    while (auto p_sz = sock.receive(buf, std::size(buf)))
    {
        std::cout << "SZ: " << p_sz << std::endl;
        std::replace(std::begin(buf), std::next(std::begin(buf), p_sz), '\0', '\n');

        std::cout.write(buf, p_sz);
        std::cout.flush();
    }
}
