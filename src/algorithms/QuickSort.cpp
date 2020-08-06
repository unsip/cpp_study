#include "Algorithms.hpp"
#include "Iterators.hpp"
#include "Utils.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <utility>

template <typename BiIt, typename Comparator>
BiIt phase::algorithms::partial_sort(BiIt begin, BiIt end, BiIt base, Comparator cmp)
{
    assert(begin != end && base != end);
    auto l = [cmp, rhv = *base](const auto& lhv){ return cmp(lhv, rhv); };
    phase::iterators::ReverseIterator<BiIt> r_begin(end);
    std::swap(*base, *r_begin);
    while (begin != r_begin.base())
    {
        begin = phase::utils::find_if(begin, r_begin.base(), std::not_fn(l));
        if (begin != r_begin.base())
            std::swap(*begin, *r_begin);
        phase::iterators::ReverseIterator<BiIt> r_end(begin);
        r_begin = phase::utils::find_if(r_begin, r_end, l);
        if (r_begin != r_end)
            std::swap(*r_begin, *begin);
    }

    return begin;
}

template <
      typename BiIt
    , typename Comparator = std::less<typename std::iterator_traits<BiIt>::value_type>
    , typename Policy = phase::utils::EndingPolicy<BiIt>
>
void phase::algorithms::quick_sort(BiIt begin, BiIt end, Comparator cmp, Policy p)
{
    static_assert(std::is_same<decltype(p({}, {})), BiIt>::value, "Bad base iterator!");
    if (begin != end)
    {
        BiIt base = p(begin, end);
        auto bisect = phase::algorithms::partial_sort(begin, end, base, cmp);
        phase::algorithms::quick_sort(begin, bisect, cmp);
        phase::algorithms::quick_sort(++bisect, end, cmp);
    }
}
