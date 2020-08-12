#pragma once

#include <iterator>
#include <random>

// TODO: Implement std::swap, std::distance, std::next
namespace phase::utils {
template <typename It>
class EndingPolicy
{
public:
    It operator ()(It, It end) const { return --end; }
};


template <typename It>
class RandomPolicy
{
private:
    std::mt19937 m_g {std::random_device {}()};
public:
    It operator ()(It begin, It end)
    {
        assert(begin != end);
        auto sz = std::distance(begin, end);
        std::uniform_int_distribution<decltype(sz)> dist(0, sz - 1);
        return std::next(begin, dist(m_g));
    }
};


template <typename It, typename Pred>
It find_if(It begin, It end, Pred pred)
{
    while (begin != end && !pred(*begin)) ++begin;
    return begin;
}
}

