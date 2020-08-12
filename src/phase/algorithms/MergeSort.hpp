#include <iterator>

namespace phase::algorithms {
namespace detail {
template <typename It, typename FwdIt, typename Comparator>
inline void merge(It lhbegin, It lhend, It rhbegin, It rhend, FwdIt output, Comparator cmp)
{
    while (lhbegin != lhend && rhbegin != rhend)
    {
        if (cmp(*lhbegin, *rhbegin))
            *output++ = *lhbegin++;
        else if (cmp(*rhbegin, *lhbegin))
            *output++ = *rhbegin++;
        else
        {
            *output++ = *lhbegin++;
            *output++ = *rhbegin++;
        }
    }

    // Basically it's std::copy.
    while (lhbegin != lhend)
        *output++ = *lhbegin++;

    while (rhbegin != rhend)
        *output++ = *rhbegin++;
}

template <typename It, typename Comparator>
It mr_impl(It begin, typename std::iterator_traits<It>::difference_type sz, Comparator cmp)
{
    if (1 < sz)
    {
        It mid = mr_impl(begin, sz / 2, cmp);
        It end = mr_impl(mid, sz - sz / 2, cmp);

        std::vector<typename std::iterator_traits<It>::value_type> tmp;
        tmp.reserve(sz);
        merge(begin, mid, mid, end, std::back_insert_iterator<decltype(tmp)>(tmp), cmp);
        std::copy(tmp.begin(), tmp.end(), begin);

        return end;
    }

    return std::next(begin, sz);
}
} // phase::algorithms::detail

template <
    typename It
  , typename Comparator = std::less<typename std::iterator_traits<It>::value_type>
>
inline void merge_sort(It begin, It end, Comparator cmp = Comparator())
{
    if (begin != end && std::next(begin) != end)
        detail::mr_impl(begin, std::distance(begin, end), cmp);
}
} // phase, algorithms

