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
} // phase::algorithms::detail

template <typename It, typename FwdIt, typename Comparator>
inline FwdIt merge_sort(It begin, It end, FwdIt fbegin, FwdIt fend, Comparator cmp)
{
    assert(std::distance(begin, end) <= std::distance(fbegin, fend) && "Not enought output memory!");
    if (begin != end || std::next(begin) != end)
    {
        auto sz = std::distance(begin, end);
        It mid = std::next(begin, sz / 2);
        FwdIt fmid = merge_sort(begin, mid, fbegin, cmp);
        fend = merge_sort(mid, end, fmid, cmp);
        detail::merge(fbegin, fmid, fmid, fend, fbegin, cmp);
    }

    return fend;
}

template <typename It, typename Comparator>
inline void merge_sort(It begin, It end, Comparator cmp)
{
    if (begin != end && std::next(begin) != end)
        mr_impl(begin, std::distance(begin, end), cmp);
}
} // phase, algorithms
