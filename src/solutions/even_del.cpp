#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>

void delete_even(std::vector<int>& vec)
{
    auto it = vec.begin();
    for (auto sz = vec.size(); sz > 0; --sz)
        if ((sz - 1) % 2 == 1)
            vec.erase(it + sz - 1);
}

template <typename FwIt>
FwIt delete_even_it(FwIt begin, FwIt end)
{
    auto it1 = begin;
    auto it2 = begin;
    while (it2 != end)
    {
        assert(std::distance(it2, begin) % 2 == 0);
        *it1++ = std::move(*it2);
        if (++it2 != end)
            ++it2;
    }
    return it1;
}

template <typename FwIt>
FwIt delete_even_it_std(FwIt begin, FwIt end)
{
    std::size_t cnt = 0;
    // Does not guarantee iteration order.
    return std::remove_if(
        begin
      , end
      , [&cnt](const auto& elem) { return ++cnt % 2 == 0; }
    );
}

int main()
{
    {
        std::vector<int> vec {};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        assert(vec.size() == 0);
    }
    {
        std::vector<int> vec = {0};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        for (auto n : vec)
            std::cout << n << " ";
        assert(vec.size() == 1);
        std::cout << std::endl;
    }
    {
        std::vector<int> vec = {0, 1, 2};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        for (auto n : vec)
            std::cout << n << " ";
        std::cout << std::endl;
        assert(vec.size() == 2);
    }
    {
        std::vector<int> vec = {0, 1, 2, 3, 4};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        for (auto n : vec)
            std::cout << n << " ";
        std::cout << std::endl;
        assert(vec.size() == 3);
    }
    {
        std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        for (auto n : vec)
            std::cout << n << " ";
        std::cout << std::endl;
        assert(vec.size() == 5);
    }
    {
        std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        auto end = delete_even_it(vec.begin(), vec.end());
        vec.erase(end, vec.end());
        for (auto n : vec)
            std::cout << n << " ";
        std::cout << std::endl;
        assert(vec.size() == 5);
    }
}
