#include <vector>
#include <tuple>
#include <cassert>
#include <utility>
#include <algorithm>
#include <iostream>

using range = std::tuple<int, int>;

range get_overlap(range lhv, range rhv)
{
    using namespace std;
    return {max(get<0>(lhv), get<0>(rhv)), min(get<1>(lhv), get<1>(rhv))};
}

bool is_overlap(range lhv, range rhv)
{
    using std::get;
    range r = get_overlap(lhv, rhv);
    return !(get<1>(r) < get<0>(r));
}

template <typename Iter>
void iter_swap(Iter lhv, Iter rhv)
{
    if (lhv != rhv)
        std::swap(*lhv, *rhv);
}

template <typename Iter, typename Cmp>
Iter remove_if(Iter first, Iter last, Cmp cmp)
{
    auto it = first;

    while (it != last)
    {
        if (!cmp(*it))
        {
            iter_swap(it, first);
            ++first;
        }
        ++it;
    }

    return first;
}

/*
 * Возвращает все пересечения lhv с отрезками в intervals.
 */
std::vector<range> get_overlaps(range lhv, std::vector<range> intervals)
{
    auto cmp = [lhv] (range value) { return !is_overlap(lhv, value); };
    auto it = ::remove_if(intervals.begin(), intervals.end(), cmp);
    intervals.erase(it, intervals.end());
    return intervals;
}

/*
 * Проверяет существуют ли пересечения у lhv с каким-либо отрезком в intervals.
 */
bool is_overlap(range lhv, std::vector<range> rhv)
{
    auto cmp = [lhv] (range curr) { return is_overlap(lhv, curr); };
    return std::find_if(rhv.begin(), rhv.end(), cmp) != rhv.end();
}

/*
 * Проверяет существуют ли двойные пересечения lhv с каким-либо отрезком в intervals.
 */
bool is_double_overlap(range lhv, std::vector<range> rhv)
{
    auto overlaps = get_overlaps(lhv, rhv);
    for (std::size_t i = 0; i < overlaps.size(); ++i)
    {
        auto vec {overlaps};
        vec.erase(std::next(vec.begin(), i));
        if (is_overlap(overlaps[i], vec))
            return true;
    }

    return false;
}

int interval_selection(std::vector<range> intervals)
{
    if (intervals.size() < 3)
        return intervals.size();

    if (intervals.size() == 3)
        return is_overlap(get_overlap(intervals[0], intervals[1]), intervals[2]) ? 2 : 3;

    int result = 0;
    for (std::size_t i = 0; i < intervals.size(); ++i)
    {
        auto vec {intervals};
        vec.erase(std::next(vec.begin(), i));
        int low_pow = interval_selection(vec);
        if (!is_double_overlap(intervals[i], vec))
            low_pow += 1;

        result = std::max(result, low_pow);
    }

    assert(1 < result);
    return result;
}

int main()
{
    {
    assert(true  == is_overlap({1, 3}, {2, 4}));
    assert(true  == is_overlap({2, 4}, {1, 3}));
    assert(false == is_overlap({1, 4}, {5, 6}));
    assert(false == is_overlap({5, 6}, {1, 4}));
    assert(true  == is_overlap({0, 1}, {1, 2}));
    assert(true  == is_overlap({1, 2}, {0, 1}));
    assert(true  == is_overlap({1, 1}, {1, 1}));
    assert(false == is_overlap({1, 1}, {2, 2}));
    assert(false == is_overlap({2, 2}, {1, 1}));
    }
    {
    assert((range{2, 2} == get_overlap({1, 2}, {2, 3})));
    assert((range{2, 2} == get_overlap({2, 3}, {1, 2})));
    }
    {
    std::vector<std::tuple<int, int>> input {};
    assert(0 == interval_selection(input));
    }
    {
    std::vector<std::tuple<int, int>> input {{1, 2}};
    assert(1 == interval_selection(input));
    }
    {
    std::vector<std::tuple<int, int>> input {{1, 2}, {2, 3}, {2, 4}};
    assert(2 == interval_selection(input));
    }
    {
    std::vector<std::tuple<int, int>> input {{1, 5}, {1, 5}, {1, 5}};
    assert(2 == interval_selection(input));
    }
    {
    std::vector<std::tuple<int, int>> input {{1, 10}, {1, 3}, {4, 6}, {7, 10}};
    std::cout << interval_selection(input) << std::endl;
    assert(4 == interval_selection(input));
    }
    {
    std::vector<std::tuple<int, int>> input {{1, 10}, {1, 3}, {3, 6}, {7, 10}};
    assert(3 == interval_selection(input));
    }
}
