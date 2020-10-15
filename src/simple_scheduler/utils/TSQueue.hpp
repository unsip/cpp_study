#pragma once

#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <queue>


// TODO: Implement TSQueue::iterator.
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

        template <typename Rep, typename Period>
        auto wait_for(std::chrono::duration<Rep, Period> dur)
        {
            return m_impl->m_cond.wait_for(m_impl->m_mut, dur);
        }

        template <typename Rep, typename Period, typename Predicate>
        auto wait_for(std::chrono::duration<Rep, Period> dur, Predicate pred)
        {
            return m_impl->m_cond.wait_for(m_impl->m_mut, dur, pred);
        }

        void push(T t)
        {
            m_impl->m_threads.push(std::move(t));
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
        {
            return m_impl->m_threads.empty();
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

