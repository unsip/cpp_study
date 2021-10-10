#include "intervals.hpp"
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>


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

/*
 * Возвращает все пересечения lhv с отрезками в intervals.
 */
std::vector<range> get_overlaps(range lhv, std::vector<range> intervals)
{
    auto cmp = [lhv] (range value) { return !is_overlap(lhv, value); };
    auto it = std::remove_if(intervals.begin(), intervals.end(), cmp);
    intervals.erase(it, intervals.end());
    return intervals;
}

/*
 * Проверяет существуют ли пересечения у lhv с каким-либо отрезком в intervals.
 */
bool is_overlap(range lhv, const std::vector<range>& rhv)
{
    auto cmp = [lhv] (range curr) { return is_overlap(lhv, curr); };
    return std::find_if(rhv.cbegin(), rhv.cend(), cmp) != rhv.cend();
}

/*
 * Проверяет существуют ли двойные пересечения lhv с каким-либо отрезком в intervals.
 */
bool is_double_overlap(range lhv, std::vector<range> rhv)
{
    auto overlaps = get_overlaps(lhv, std::move(rhv));
    for (auto it = overlaps.cbegin(), last = overlaps.cend(); it != last; ++it)
    {
        std::vector<range> vec;
        // copy all elements exept current one
        vec.reserve(overlaps.size() - 1);
        vec.insert(vec.cend(), overlaps.cbegin(), it);
        vec.insert(vec.cend(), std::next(it), overlaps.cend());

        if (is_overlap(*it, vec))
            return true;
    }

    return false;
}

int interval_selection(const std::vector<range>& intervals)
{
    if (intervals.size() < 3)
        return intervals.size();

    int result = 0;
    for (auto it = intervals.cbegin(), last = intervals.cend(); it != last; ++it)
    {
        std::vector<range> vec;
        // copy all elements exept current one
        vec.reserve(intervals.size() - 1);
        vec.insert(vec.cend(), intervals.cbegin(), it);
        vec.insert(vec.cend(), std::next(it), intervals.cend());

        int low_pow = interval_selection(vec);
        if (!is_double_overlap(*it, std::move(vec)))
            low_pow += 1;

        result = std::max(result, low_pow);
    }

    assert(1 < result);
    return result;
}

