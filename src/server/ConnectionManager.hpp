#pragma once

#include "TcpSocket.hpp"

#include <list>
#include <string>
#include <mutex>
#include <optional>


class ConnectionManager
{
private:
    std::optional<phase::network::TCPSocket> m_acceptor;
    std::mutex m_mut;
    // Note: List iterators (captured by Connection objects) won't be invalidated upon modification.
    std::list<phase::network::TCPSocket> m_connections;
    ConnectionManager() = default;

public:
    // Note: All Connection's should be destroyed before ConnectionManager.
    // It's true since ConnectionManager is a singletone with static placement.
    // Be careful not to create static Connection class.
    class Connection
    {
    private:
        std::optional<std::list<phase::network::TCPSocket>::iterator> m_sock_it;
    public:
        Connection(std::list<phase::network::TCPSocket>::iterator it)
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

