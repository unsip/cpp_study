#include <iostream>
#include <vector>

template <typename It, typename Comparator>
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

template <typename It, typename FwdIt, typename Comparator>
FwdIt merge_sort(It begin, It end, FwdIt fbegin, Comparator cmp)
{
    assert(std::distance(begin, end) <= std::distance(fbegin, fend) && "Not enought output memory!");
    if (begin != end || std::next(begin) != end)
    {
        auto sz = std::distance(begin, end);
        It mid = std::next(begin, sz / 2);
        FwdIt fmid = merge_sort(begin, mid, fbegin, cmp);
        fend = merge_sort(mid, end, fmid, cmp);
        merge(fbegin, fmid, fmid, fend, fbegin, cmp);
    }

    return fend;
}

int main()
{
    {
    merge_sort(nullptr, 0);
    }
    {
    std::vector<int> arr = {1};
    std::vector<int> res_arr;
    res_arr.reserve(arr.size());
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
    merge_sort(arr.begin(), arr.end());
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
