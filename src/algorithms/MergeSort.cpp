#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cassert>

template <typename It, typename OutputIt, typename Comparator>
void merge(It lhbegin, It lhend, It rhbegin, It rhend, OutputIt output, Comparator cmp)
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

template <typename It, typename Comparator>
void merge_sort(It begin, It end, Comparator cmp)
{
    if (begin != end && std::next(begin) != end)
        mr_impl(begin, std::distance(begin, end), cmp);
}

int main()
{
    auto cmp = [](auto lhv, auto rhv){ return lhv < rhv; };
    {
    std::vector<int> arr;
    merge_sort(arr.begin(), arr.end(), cmp);
    }
    {
    std::vector<int> arr = {1};
    merge_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert(arr == std::vector<int> ({1}));
    }
    {
    std::vector<int> arr = {1, 2};
    merge_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 2}}));
    }
    {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    merge_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 1, 1, 1}}));
    }
    {
    std::vector<int> arr = {3, 1, 2, 1, 2, 3};
    merge_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 2, 2, 3, 3}}));
    }
    {
    std::vector<int> arr {1, 2, 3, 4};
    merge_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 2, 3, 4}));
    }
}
