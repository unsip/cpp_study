#pragma once

#include <utility>
#include <iterator>
#include <cassert>

namespace phase::algorithms::heap {
namespace detail {
std::size_t parent(std::size_t curr) { return curr == 0 ? 0 : (curr - 1) / 2; }
std::size_t left(std::size_t curr) { return curr * 2 + 1; }
std::size_t right(std::size_t curr) { return curr * 2 + 2; }
} // namespace phase::algorithms::detail

template <typename RAIt, typename Comparator>
inline void heapify_up(RAIt btop, std::size_t curr, Comparator cmp)
{
    while (curr > 0)
    {
        std::size_t p = detail::parent(curr);
        if (cmp(btop[curr], btop[p]))
            std::swap(btop[curr], btop[p]);
        curr = p;
    }
}

template <typename RAIt, typename Comparator>
inline void build_heap(RAIt first, RAIt last, Comparator cmp)
{
    for (auto r_it = std::make_reverse_iterator(last), r_end = std::make_reverse_iterator(first); r_it != r_end ; ++r_it)
        heapify_up(first, std::distance(first, r_it.base()),  cmp);
}

template <typename RAIt, typename Comparator>
inline void heapify_down(RAIt first, RAIt last, std::size_t curr, Comparator cmp)
{
    auto get_size = [&first, &last]
    {
        auto sz = std::distance(first, last);
        assert(sz >= 0);
        return static_cast<std::size_t>(sz);
    };
    auto sz = get_size();

    while (curr < sz)
    {
        std::size_t l = detail::left(curr);
        std::size_t r = detail::right(curr);
        std::size_t smallest;
        assert(l < r);
        if (r < sz)
            smallest = cmp(first[l], first[r]) ? l : r;
        else if (l < sz)
            smallest = l;
        else
            break;

        if (!(cmp(first[smallest], first[curr])))
            break;

        std::swap(first[smallest], first[curr]);
        curr = smallest;
    }
}

template <typename RAIt, typename Comparator>
inline RAIt pop(RAIt first, RAIt last, Comparator cmp)
{
    if (first == last)
        return last;

    auto new_last = std::prev(last);
    std::swap(*first, *new_last);
    heapify_down(first, new_last, 0, cmp);
    return new_last;
}
} // namespace phase, algorithms, heap

