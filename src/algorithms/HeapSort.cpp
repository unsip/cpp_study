#include <vector>
#include <cassert>
#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>

namespace {
std::size_t parent(std::size_t curr) { return curr == 0 ? 0 : (curr - 1) / 2; }
std::size_t left(std::size_t curr) { return curr * 2 + 1; }
std::size_t right(std::size_t curr) { return curr * 2 + 2; }
}

template <typename Comparator>
void heapify_up(int* btop, std::size_t curr, Comparator cmp)
{
    while (curr > 0)
    {
        std::size_t p = parent(curr);
        if (cmp(btop[curr], btop[p]))
            std::swap(btop[curr], btop[p]);
        curr = p;
    }
}

template <typename Comparator>
void heapify_down(int* btop, std::size_t sz, std::size_t curr, Comparator cmp)
{
    while (curr < sz)
    {
        std::size_t l = left(curr);
        std::size_t r = right(curr);
        std::size_t smallest;
        assert(l < r);
        if (r < sz)
            smallest = cmp(btop[l], btop[r]) ? l : r;
        else if (l < sz)
            smallest = l;
        else
            break;

        if (!(cmp(btop[smallest], btop[curr])))
            break;

        std::swap(btop[smallest], btop[curr]);
        curr = smallest;
    }
}

template <typename Comparator>
void build_heap(int* btop, std::size_t sz, Comparator cmp)
{
    for (auto i = sz - 1; i != 0; --i)
        heapify_up(btop, i, cmp);
}

template <typename Comparator>
std::size_t pop(int* btop, std::size_t sz, Comparator cmp)
{
    if (!sz || !btop)
        return sz;

    std::swap(btop[0], btop[sz - 1]);
    heapify_down(btop, sz - 1, 0, cmp);

    return sz - 1;
}

template <typename Comparator = std::less<int>>
void heapsort(int* btop, std::size_t sz, Comparator cmp = Comparator{})
{
    if (btop)
    {
        auto not_cmp = std::not_fn(std::move(cmp));
        build_heap(btop, sz, not_cmp);
        while (sz != 0)
            sz = pop(btop, sz, not_cmp);
    }
}


template <typename RAIt, typename Comparator>
void heapify_up(RAIt btop, std::size_t curr, Comparator cmp)
{
    while (curr > 0)
    {
        std::size_t p = parent(curr);
        if (cmp(btop[curr], btop[p]))
            std::swap(btop[curr], btop[p]);
        curr = p;
    }
}

template <typename RAIt, typename Comparator>
void build_heap(RAIt first, RAIt last, Comparator cmp)
{
    for (auto r_it = std::make_reverse_iterator(last), r_end = std::make_reverse_iterator(first); r_it != r_end ; ++r_it)
        heapify_up(first, std::distance(first, r_it.base()),  cmp);
}

template <typename RAIt, typename Comparator>
void heapify_down(RAIt first, RAIt last, std::size_t curr, Comparator cmp)
{
    auto sz = std::distance(first, last);
    while (curr < sz)
    {
        std::size_t l = left(curr);
        std::size_t r = right(curr);
        std::size_t smallest;
        assert(l < r);
        if (r < sz)
            smallest = cmp(first[l], first[r]) ? l : r;
        else if (l < sz)
            smallest = l;
        else
            break;

        if (!(cmp(first[smallest], first[curr])))
            break;

        std::swap(first[smallest], first[curr]);
        curr = smallest;
    }
}

template <typename RAIt, typename Comparator>
RAIt pop(RAIt first, RAIt last, Comparator cmp)
{
    if (first == last)
        return last;

    auto new_last = std::prev(last);
    std::swap(*first, *new_last);
    heapify_down(first, new_last, 0, cmp);
    return new_last;
}

template <typename RAIt, typename Comparator = std::less<typename std::iterator_traits<RAIt>::value_type>>
void heapsort(RAIt first, RAIt last, Comparator cmp = Comparator{})
{
    if (first)
    {
        build_heap(first, last, cmp);
        while (first != last)
        {
#ifndef NDEBUG
            auto top = *first;
#endif
            last = pop(first, last);
            assert(top == *last);
        }
    }

}

int main()
{
    {
    heapsort(nullptr, 0);
    }
    {
    std::vector<int> arr = {1};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert(arr == std::vector<int> ({1}));
    }
    {
    std::vector<int> arr = {1, 2};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 2}}));
    }
    {
    std::vector<int> arr = {4, 2, 5, 3};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{2, 3, 4, 5}}));
    }
    {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 1, 1, 1}}));
    }
    {
    std::vector<int> arr = {3, 1, 2, 1, 2, 3};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 2, 2, 3, 3}}));
    }
    {
    std::vector<int> arr {1, 2, 3, 4};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 2, 3, 4}));
    }
    {
    std::vector<int> arr {1, 4, 3, 4};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 3, 4, 4}));
    }
    {
    std::vector<int> arr {9, 6, 1};
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 6, 9}));
    }
    {
    std::vector<int> arr {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    heapsort(arr.data(), arr.size());
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
    heapsort(arr.data(), arr.size());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::sort(arr2.begin(), arr2.end());
    assert(arr == arr2);
    }
}
