#pragma once
#include <chrono>
#include <iterator>
#include <ranges>

template <std::forward_iterator FwIt>
class TimePointMock : public Generator<std::chrono::steady_clock::time_point>
{
    mutable FwIt m_begin;
    FwIt m_end;
public:
    template <std::ranges::forward_range Rng>
    TimePointMock(const Rng& rng) : m_begin(begin(rng)), m_end(end(rng))
    {}

    std::chrono::steady_clock::time_point get() const override
    {
        assert(m_begin != m_end && "Fucked up iters!");
        return *m_begin++;
    }
};

template <std::ranges::forward_range Rng>
TimePointMock(const Rng&) -> TimePointMock<decltype(begin(std::declval<Rng>()))>;
