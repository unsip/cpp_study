#include "TcpSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <cerrno>
#include <cassert>
#include <stdexcept>


/**
 * Utility object for representing system locale and converting any given
 * system error to C-string.
 *
 * @todo: Move to Utils library.
 */
class Locale
{
private:
    locale_t m_loc;

public:
    Locale()
    {
        m_loc = newlocale(
            LC_CTYPE_MASK | LC_NUMERIC_MASK | LC_TIME_MASK
            | LC_COLLATE_MASK | LC_MONETARY_MASK | LC_MESSAGES_MASK
          , ""
          , locale_t{0}
        );

        if (locale_t{0} == m_loc && ENOMEM == errno)
            throw std::runtime_error("ENOMEM");

        assert(m_loc && "Inproper locale mask!");
    }

    Locale(const Locale&) = delete;
    Locale& operator= (const Locale&) = delete;

    const char* err2str(int errnum)
    {
        return strerror_l(errnum, m_loc);
    }

    ~Locale() { freelocale(m_loc); }
};

/**
 * Utility function for converting any system `errnum` to C-string.
 */
const char* err2str(int errnum)
{
    // Initialized Locale as static object so that it will be created only
    // once.
    static Locale loc{};
    return loc.err2str(errnum);
}

// @todo: Wrap socket initialization with external object in order to ease
// error handling in constructor.
TCPSocket::TCPSocket()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_fd)
    {
        switch (errno)
        {
            case EMFILE:
            case ENFILE:
            case ENOMEM:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }

    int optval = 1;
    int res = setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if (-1 == res)
    {
        switch (errno)
        {
            case ENOMEM:
            case ENOBUFS:
            {
                ::close(m_fd);
                throw std::runtime_error(err2str(errno));
            }
        }
        assert(false && "Can't reach here!");
    }
}

TCPSocket::~TCPSocket()
{
    close();
}


/// @todo: Implement custom exception.
void TCPSocket::bind(std::string addr, std::uint16_t port)
{
    in_addr ip_addr;

    if (!inet_aton(addr.c_str(), &ip_addr))
        throw std::runtime_error("Invalid IP address: '" + addr + "'");

    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr = ip_addr;

    int rc = ::bind(m_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa);
    if (-1 == rc)
    {
        switch (errno)
        {
            case EACCES:
            case EADDRINUSE:
            case EINVAL:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }
}

void TCPSocket::listen()
{
    int rc = ::listen(m_fd, SOMAXCONN);
    if (-1 == rc)
    {
        if (EADDRINUSE == errno)
            throw std::runtime_error(err2str(errno));
        assert(false && "Can't reach here!");
    }
}

std::size_t TCPSocket::receive(char* buf, std::size_t len)
{
    if (buf == nullptr)
        throw std::runtime_error("Invalid buffer value: nullptr");

    ssize_t res = recv(m_fd, buf, len, 0);
    if (-1 == res)
    {
        switch (errno)
        {
            case ECONNRESET:
            case EIO:
            case ENOBUFS:
            case ENOMEM:
            case ENOTCONN:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }

    assert(res >= 0);

    return std::size_t(res);
}

// @todo: Figure out why assert is triggered upon interupted connection from client-side
void TCPSocket::send(const char* buf, std::size_t len)
{
    if (buf == nullptr)
        throw std::runtime_error("Invalid buffer value: nullptr");

    ssize_t res = ::send(m_fd, buf, len, MSG_NOSIGNAL);

    if (-1 == res)
    {
        switch (errno)
        {
            case ECONNRESET:
            case EDESTADDRREQ:
            case ENOTCONN:
            case EACCES:
            case EIO:
            case ENETDOWN:
            case ENETUNREACH:
            case ENOBUFS:
            case EPIPE:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }

    assert(res >= 0 && "Sent data is less than 0!");
    assert(std::size_t(res) == len && "Sent data is less than specified length!");
}

void TCPSocket::connect(std::string addr, std::uint16_t port)
{
    in_addr ip_addr;
    if (!inet_aton(addr.c_str(), &ip_addr))
        throw std::runtime_error("Invalid IP address: '" + addr + "'");

    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr = ip_addr;

    int rc = ::connect(m_fd, reinterpret_cast<const sockaddr*>(&sa), sizeof sa);
    if (-1 == rc)
    {
        switch (errno)
        {
            case EADDRNOTAVAIL:
            case EALREADY:
            case ECONNREFUSED:
            case EISCONN:
            case ENETUNREACH:
            case EHOSTUNREACH:
            case EPROTOTYPE:
            case ETIMEDOUT:
            case EACCES:
            case EADDRINUSE:
            case ENOBUFS:
            case EOPNOTSUPP:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }
}

TCPSocket TCPSocket::accept()
{
    int rc = ::accept(m_fd, nullptr, 0);
    if (-1 == rc)
    {
        switch (errno)
        {
            case ECONNABORTED:
            case EMFILE:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case EOPNOTSUPP:
            case EPROTO:
                throw std::runtime_error(err2str(errno));
        }
        assert(false && "Can't reach here!");
    }

    return TCPSocket(rc);
}

void TCPSocket::close() noexcept
{
    // Prevent closing moved socket.
    if (-1 != m_fd)
    {
        int res = ::close(m_fd);
        assert(res != -1 && "Can't reach here!");
    }
}
