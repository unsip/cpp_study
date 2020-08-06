#pragma once

#include "Utils.hpp"

#include <iterator>

namespace phase::algorithms {
template <
    typename RAIt
  , typename Comparator = std::less<typename std::iterator_traits<RAIt>::value_type>
>
void heapsort(RAIt first, RAIt last, Comparator cmp = Comparator{});

template <typename BiIt, typename Comparator>
BiIt partial_sort(BiIt begin, BiIt end, BiIt base, Comparator cmp);

template <
      typename BiIt
    , typename Comparator = std::less<typename std::iterator_traits<BiIt>::value_type>
    , typename Policy = phase::utils::EndingPolicy<BiIt>
>
void quick_sort(BiIt begin, BiIt end, Comparator cmp = Comparator(), Policy p = Policy());

template <typename It, typename FwdIt, typename Comparator>
FwdIt merge_sort(It begin, It end, FwdIt fbegin, Comparator cmp);
}
