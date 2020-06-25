#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iterator>

void swap(int& l, int& r)
{
    int tmp = r;
    r = l;
    l = tmp;
}

// TODO: Generalize for two iterators, comparator and strategy to choose base element.
//
// - Выразить все стандартные операции сравнения (<, >, ==, !=, <=, =>)
//      - Через < и !
//      - Через <=
std::size_t partial_sort(int* arr, std::size_t sz, std::size_t i)
{
    assert(i < sz);
    std::size_t li = 0;
    std::size_t ri = sz - 1;
    int base = arr[i];
    swap(arr[i], arr[ri]);

    while (li != ri)
    {
        // Find
        for (; li != ri; ++li)
            if (!(arr[li] < base))
            {
                swap(arr[li], arr[ri]);
                break;
            }

        for (; ri != li; --ri)
            if (arr[ri] < base)
            {
                swap(arr[ri], arr[li]);
                break;
            }
    }

    return li;
}

void quick_sort(int* arr, std::size_t sz)
{
    if (sz != 0)
    {
        std::size_t bisect = partial_sort(arr, sz, sz - 1);
        std::size_t lsz = bisect;
        std::size_t rsz = sz - bisect - 1;
        quick_sort(arr, lsz);
        quick_sort(arr + bisect + 1, rsz);
    }
}

int main()
{
    {
    quick_sort(nullptr, 0);
    }
    {
    std::vector<int> arr = {1};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert(arr == std::vector<int> ({1}));
    }
    {
    std::vector<int> arr = {1, 2};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 2}}));
    }
    {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 1, 1, 1}}));
    }
    {
    std::vector<int> arr = {3, 1, 2, 1, 2, 3};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 2, 2, 3, 3}}));
    }
    {
    std::vector<int> arr {1, 2, 3, 4};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 2, 3, 4}));
    }
    {
    std::vector<int> arr {1, 4, 3, 4};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 3, 4, 4}));
    }
    {
    std::vector<int> arr {9, 6, 1};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 6, 9}));
    }
    {
    std::vector<int> arr {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
    }
    {
    std::vector<int> arr
    {   9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 111, 1314, 5155, 5123, 555, 5111, 999, 313, 31241, 4124, 41425, 512414, 313,
        3213, 425, 52523, 523423, 62325, 42432, 5299, 2834, -123, -315151, 5123, 5, 0, 31, -1,
        3213, 425, -52523, 523423, -62325, 42432, 5299, 2834, 123, -315151, 5123, 5, 0, 31, -1
    };
    std::vector<int> arr2
    {   9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 111, 1314, 5155, 5123, 555, 5111, 999, 313, 31241, 4124, 41425, 512414, 313,
        3213, 425, 52523, 523423, 62325, 42432, 5299, 2834, -123, -315151, 5123, 5, 0, 31, -1,
        3213, 425, -52523, 523423, -62325, 42432, 5299, 2834, 123, -315151, 5123, 5, 0, 31, -1
    };
    quick_sort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::sort(arr2.begin(), arr2.end());
    assert(arr == arr2);
    }
}
