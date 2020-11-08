#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <exception>

// TODO: Separate namespace.
class SocketClosedError : public std::exception
{
    const std::string m_what_msg;
public:
    SocketClosedError(const std::string& what_msg)
        : m_what_msg(what_msg)
    {}
    const char* what() const noexcept override { return m_what_msg.c_str(); }
};


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
