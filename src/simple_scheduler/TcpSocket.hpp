#pragma once

#include <string>
#include <cstdint>
#include <chrono>

class TCPSocket
{
private:
    int m_fd;

    TCPSocket(int fd) noexcept
        : m_fd(fd)
    {}

public:
    TCPSocket();

    // Suppress copy constructor and assign.
    TCPSocket(const TCPSocket&) = delete;
    TCPSocket& operator= (const TCPSocket&) = delete;

    // Socket should be moveable to pass it into function or return from
    // factory. Marking rhv.m_fd as a moved object. See destructor.
    TCPSocket(TCPSocket&& rhv) noexcept
        : m_fd(rhv.m_fd)
    {
        rhv.m_fd = -1;
    }

    TCPSocket& operator= (TCPSocket&& rhv) noexcept
    {
        m_fd = rhv.m_fd;
        rhv.m_fd = -1;
        return *this;
    }

    ~TCPSocket();

    void bind(std::string addr, std::uint16_t port);
    void connect(std::string addr, std::uint16_t port);
    void listen();
    std::size_t receive(char* buf, std::size_t len);
    void send(const char* buf, std::size_t len);
    void close() noexcept;

    TCPSocket accept();
};
