#include "Iterators.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <iterator>
#include <utility>
#include <cassert>

namespace phase::algorithms {
template <typename BiIt, typename Comparator>
inline BiIt partial_sort(BiIt begin, BiIt end, BiIt base, Comparator cmp)
{
    assert(begin != end && base != end);
    auto l = [cmp, rhv = *base](const auto& lhv){ return cmp(lhv, rhv); };
    iterators::ReverseIterator<BiIt> r_begin(end);
    std::swap(*base, *r_begin);
    while (begin != r_begin.base())
    {
        begin = utils::find_if(begin, r_begin.base(), std::not_fn(l));
        if (begin != r_begin.base())
            std::swap(*begin, *r_begin);
        iterators::ReverseIterator<BiIt> r_end(begin);
        r_begin = utils::find_if(r_begin, r_end, l);
        if (r_begin != r_end)
            std::swap(*r_begin, *begin);
    }

    return begin;
}

template <
      typename BiIt
    , typename Comparator = std::less<typename std::iterator_traits<BiIt>::value_type>
    , typename Policy = utils::EndingPolicy<BiIt>
>
inline void quick_sort(BiIt begin, BiIt end, Comparator cmp = Comparator(), Policy p = Policy())
{
    static_assert(std::is_same<decltype(p({}, {})), BiIt>::value, "Bad base iterator!");
    if (begin != end)
    {
        BiIt base = p(begin, end);
        auto bisect = phase::algorithms::partial_sort(begin, end, base, cmp);
        quick_sort(begin, bisect, cmp);
        quick_sort(++bisect, end, cmp);
    }
}
} // namespace phase, algorithms

