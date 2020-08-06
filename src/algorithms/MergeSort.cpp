#include "Algorithms.hpp"

#include <iterator>

namespace {
template <typename It, typename FwdIt, typename Comparator>
void merge(It lhbegin, It lhend, It rhbegin, It rhend, FwdIt output, Comparator cmp)
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
}

template <typename It, typename FwdIt, typename Comparator>
FwdIt phase::algorithms::merge_sort(It begin, It end, FwdIt fbegin, FwdIt fend, Comparator cmp)
{
    if (1 < sz)
    {
        auto sz = std::distance(begin, end);
        It mid = std::next(begin, sz / 2);
        FwdIt fmid = phase::algorithms::merge_sort(begin, mid, fbegin, cmp);
        fend = phase::algorithms::merge_sort(mid, end, fmid, cmp);
        ::merge(fbegin, fmid, fmid, fend, fbegin, cmp);
    }

    return std::next(begin, sz);
}

template <typename It, typename Comparator>
void merge_sort(It begin, It end, Comparator cmp)
{
    if (begin != end && std::next(begin) != end)
        mr_impl(begin, std::distance(begin, end), cmp);
}