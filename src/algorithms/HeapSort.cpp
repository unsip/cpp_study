#include "Algorithms.hpp"

#include <cassert>
#include <utility>
#include <iterator>
#include <algorithm>

namespace {
std::size_t parent(std::size_t curr) { return curr == 0 ? 0 : (curr - 1) / 2; }
std::size_t left(std::size_t curr) { return curr * 2 + 1; }
std::size_t right(std::size_t curr) { return curr * 2 + 2; }

template <typename RAIt, typename Comparator>
void heapify_up(RAIt btop, std::size_t curr, Comparator cmp)
{
    while (curr > 0)
    {
        std::size_t p = parent(curr);
        if (cmp(btop[curr], btop[p]))
            std::swap(btop[curr], btop[p]);
        curr = p;
    }
}

template <typename RAIt, typename Comparator>
void build_heap(RAIt first, RAIt last, Comparator cmp)
{
    for (auto r_it = std::make_reverse_iterator(last), r_end = std::make_reverse_iterator(first); r_it != r_end ; ++r_it)
        heapify_up(first, std::distance(first, r_it.base()),  cmp);
}

template <typename RAIt, typename Comparator>
void heapify_down(RAIt first, RAIt last, std::size_t curr, Comparator cmp)
{
    auto sz = std::distance(first, last);
    while (curr < sz)
    {
        std::size_t l = left(curr);
        std::size_t r = right(curr);
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
RAIt pop(RAIt first, RAIt last, Comparator cmp)
{
    if (first == last)
        return last;

    auto new_last = std::prev(last);
    std::swap(*first, *new_last);
    heapify_down(first, new_last, 0, cmp);
    return new_last;
}
}

template <typename RAIt, typename Comparator = std::less<typename std::iterator_traits<RAIt>::value_type>>
void phase::algorithms::heapsort(RAIt first, RAIt last, Comparator cmp)
{
    if (first)
    {
        ::build_heap(first, last, cmp);
        while (first != last)
        {
#ifndef NDEBUG
            auto top = *first;
#endif
            last = ::pop(first, last);
            assert(top == *last);
        }
    }
}
