#pragma once

#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <unordered_map>


template <typename Key, typename Value>
class TSQueue
{
private:
    struct Impl
    {
        std::mutex m_mut;
        std::condition_variable_any m_cond;
        std::unordered_map<Key, Value> m_threads;
    };
    std::shared_ptr<Impl> m_impl;
    using ContainerType = decltype(Impl::m_threads);

public:
    using TSQueueIt = typename ContainerType::iterator;
    using ConstTSQueueIt = typename ContainerType::const_iterator;
    using ValueType = typename ContainerType::value_type;

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

        TSQueueIt begin()
        {
            return m_impl->m_threads.begin();
        }

        TSQueueIt end()
        {
            return m_impl->m_threads.end();
        }

        ConstTSQueueIt cbegin() const
        {
            return m_impl->m_threads.cbegin();
        }

        ConstTSQueueIt cend() const
        {
            return m_impl->m_threads.cend();
        }

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

        template <typename... Args>
        auto emplace(Args&&... args) -> decltype(auto)
        {
            return m_impl->m_threads.emplace(std::forward<Args>(args)...);
        }

        auto erase(ConstTSQueueIt it) -> decltype(auto)
        {
            m_impl->m_threads.erase(it);
        }

        TSQueueIt find(const Key& key)
        {
            return m_impl->m_threads.find(key);
        }

        ConstTSQueueIt find(const Key& key) const
        {
            return m_impl->m_threads.find(key);
        }

        auto size() const -> decltype(auto)
        {
            return m_impl->m_threads.size();
        }

        bool empty() const
        {
            return m_impl->m_threads.empty();
        }

        void swap(std::unordered_map<Key, Value>& q) noexcept
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

