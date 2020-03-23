#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <utility>
#include <cassert>


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

#if 0
int main()
{
    using namespace std;
    ofstream fout(getenv("OUTPUT_PATH"));

    int s;
    cin >> s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int s_itr = 0; s_itr < s; s_itr++)
    {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<range> intervals;
        intervals.reserve(n);
        for (int intervals_row_itr = 0; intervals_row_itr < n; intervals_row_itr++)
        {
            range rng;
            cin >> get<0>(rng);
            cin >> get<1>(rng);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            intervals.emplace_back(rng);
        }

        int result = interval_selection(intervals);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
#endif

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

    assert(0 == interval_selection({}));
    assert(1 == interval_selection({{1, 2}}));

    assert(2 == interval_selection({{1, 2}, {1, 2}}));
    assert(2 == interval_selection({{1, 2}, {2, 4}}));
    assert(2 == interval_selection({{2, 4}, {1, 2}}));
    assert(2 == interval_selection({{1, 3}, {4, 6}}));
    assert(2 == interval_selection({{4, 6}, {1, 3}}));

    assert(2 == interval_selection({{1, 2}, {2, 3}, {2, 4}}));
    assert(2 == interval_selection({{1, 2}, {2, 4}, {2, 3}}));
    assert(2 == interval_selection({{2, 3}, {1, 2}, {2, 4}}));
    assert(2 == interval_selection({{2, 3}, {2, 4}, {1, 2}}));
    assert(2 == interval_selection({{2, 4}, {1, 2}, {2, 3}}));
    assert(2 == interval_selection({{2, 4}, {2, 3}, {1, 2}}));

    assert(2 == interval_selection({{1, 5}, {1, 5}, {1, 5}}));

    assert(3 == interval_selection({{1, 3}, {4, 5}, {6, 7}}));
    assert(3 == interval_selection({{1, 3}, {6, 7}, {4, 5}}));
    assert(3 == interval_selection({{4, 5}, {1, 3}, {6, 7}}));
    assert(3 == interval_selection({{4, 5}, {6, 7}, {1, 3}}));
    assert(3 == interval_selection({{6, 7}, {1, 3}, {4, 5}}));
    assert(3 == interval_selection({{6, 7}, {4, 5}, {1, 3}}));

    assert(4 == interval_selection({{1, 10}, {1, 3}, {4, 6}, {7, 10}}));
    assert(3 == interval_selection({{1, 10}, {1, 3}, {3, 6}, {7, 10}}));

    assert(4 == interval_selection({{1, 2}, {2, 3}, {3, 4}, {4, 5}}));
    assert(2 == interval_selection({{1,10}, {2, 9}, {3, 8}, {4, 7}}));
    assert(2 == interval_selection({{1,10}, {2,11}, {3,12}, {4,14}}));
    assert(3 == interval_selection({{1, 5}, {3, 5}, {3, 3}, {4, 4}}));

    assert(5 == interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}}));
    assert(6 == interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}, {1, 5}}));

    // 1234567890
    // ----------
    // ------
    //    ------
    // ---
    //        --
    // -       -
    //  -     -
    assert(6 == interval_selection({{1,10}, {1, 6}, {4,10}, {1, 3}, {8,10}, {1, 1}, {2, 2}, {9, 9}, {10,10}}));

    // 1234567890123456789
    // ----------- --  ---
    // ----------  ---- -- 
    //         ----------
    assert(6 == interval_selection({{1,11}, {1,10}, {9,18}, {13,14}, {17,19}, {13,16}, {18,19}}));

    // 1234567890123456789
    // ------------
    // --------
    //        -----
    //         -
    //           --
    assert(4 == interval_selection({{1,12}, {1, 8}, {8,12}, {9, 9}, {1, 2}}));

    // 1234567890
    // -------
    // -
    //  -
    //   -
    //    -
    //     -
    //   --------
    assert(6 == interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}, {1, 7}, {3, 10}}));
}
