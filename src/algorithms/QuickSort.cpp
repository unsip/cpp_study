#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <random>


template <typename It>
class EndingPolicy
{
public:
    It operator ()(It begin, It end) const { return --end; }
};


template <typename It>
class RandomPolicy
{
private:
    std::mt19937 m_g {std::random_device {}()};
public:
    It operator ()(It begin, It end)
    {
        assert(begin != end);
        auto sz = std::distance(begin, end);
        std::uniform_int_distribution<decltype(sz)> dist(0, sz - 1);
        return std::next(begin, dist(m_g));
    }
};


template <typename It>
class ReverseIterator
{
private:
    It m_it;
public:
    ReverseIterator(It it) : m_it(it) {}
    ReverseIterator& operator ++() { --m_it; return *this; }
    ReverseIterator operator ++(int) { return ReverseIterator(m_it--); }
    typename std::iterator_traits<It>::value_type& operator *() { It tmp = m_it; return *--tmp; }
    bool operator ==(const ReverseIterator& rhv) { return m_it == rhv.m_it; }
    bool operator !=(const ReverseIterator& rhv) { return !(*this == rhv); }
    It base() { return m_it; }
};


template <typename BiIt, typename Pred>
BiIt find_if(BiIt begin, BiIt end, Pred pred)
{
    while (begin != end && !pred(*begin)) ++begin;
    return begin;
}

template <typename BiIt, typename Comparator>
BiIt partial_sort(BiIt begin, BiIt end, BiIt base, Comparator cmp)
{
    assert(begin != end && base != end);
    auto l = [cmp, rhv = *base](const auto& lhv){ return cmp(lhv, rhv); };
    ReverseIterator<BiIt> r_begin(end);
    std::swap(*base, *r_begin);
    while (begin != r_begin.base())
    {
        begin = ::find_if(begin, r_begin.base(), std::not_fn(l));
        if (begin != r_begin.base())
            std::swap(*begin, *r_begin);
        ReverseIterator<BiIt> r_end(begin);
        r_begin = ::find_if(r_begin, r_end, l);
        if (r_begin != r_end)
            std::swap(*r_begin, *begin);
    }

    return begin;
}

template <
      typename BiIt
    , typename Comparator = std::less<typename std::iterator_traits<BiIt>::value_type>
    , typename Policy = EndingPolicy<BiIt>
>
void quick_sort(
          BiIt begin
        , BiIt end
        , Comparator cmp = Comparator()
        , Policy p = Policy()
)
{
    static_assert(std::is_same<decltype(p({}, {})), BiIt>::value, "Bad base iterator!");
    if (begin != end)
    {
        BiIt base = p(begin, end);
        auto bisect = ::partial_sort(begin, end, base, cmp);
        quick_sort(begin, bisect, cmp);
        quick_sort(++bisect, end, cmp);
    }
}

int main()
{
    auto cmp = [](int lhv, int rhv){ return lhv < rhv; };
    {
    quick_sort(nullptr, 0);
    }
    {
    std::vector<int> arr = {1};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert(arr == std::vector<int> ({1}));
    }
    {
    std::vector<int> arr = {1, 2};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 2}}));
    }
    {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    quick_sort(arr.begin(), arr.end());
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 1, 1, 1}}));
    }
    {
    std::vector<int> arr = {3, 1, 2, 1, 2, 3};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {{1, 1, 2, 2, 3, 3}}));
    }
    {
    std::vector<int> arr {1, 2, 3, 4};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 2, 3, 4}));
    }
    {
    std::vector<int> arr {1, 4, 3, 4};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 3, 4, 4}));
    }
    {
    std::vector<int> arr {9, 6, 1};
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    assert((arr == std::vector<int> {1, 6, 9}));
    }
    {
    std::vector<int> arr {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    quick_sort(arr.begin(), arr.end(), cmp, RandomPolicy<decltype(arr)::iterator>());
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
    quick_sort(arr.begin(), arr.end(), cmp);
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::sort(arr2.begin(), arr2.end());
    assert(arr == arr2);
    }
}
