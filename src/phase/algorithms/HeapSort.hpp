#include "Heap.hpp"

// TODO: Implement your own std::less
#include <functional>
#include <iterator>
#include <cassert>

namespace phase::algorithms {
template <typename RAIt, typename Comparator = std::less<typename std::iterator_traits<RAIt>::value_type>>
inline void heapsort(RAIt first, RAIt last, Comparator cmp = Comparator())
{
    heap::build_heap(first, last, cmp);
    while (first != last)
    {
#ifndef NDEBUG
        auto top = *first;
#endif
        last = heap::pop(first, last, cmp);
        assert(top == *last);
    }
}
} // namespace phase, algorithms

