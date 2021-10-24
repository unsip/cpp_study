#include <gtest/gtest.h>
#include "../intervals.hpp"

class IntervalsTest : public ::testing::Test
{
public:
};

TEST_F(IntervalsTest, IsOverlapSimplTest)
{
    EXPECT_TRUE(is_overlap({1, 3}, {2, 4}));
    EXPECT_TRUE(is_overlap({2, 4}, {1, 3}));
    EXPECT_FALSE(is_overlap({1, 4}, {5, 6}));
    EXPECT_FALSE(is_overlap({5, 6}, {1, 4}));
    EXPECT_TRUE(is_overlap({0, 1}, {1, 2}));
    EXPECT_TRUE(is_overlap({1, 2}, {0, 1}));
    EXPECT_TRUE(is_overlap({1, 1}, {1, 1}));
    EXPECT_FALSE(is_overlap({1, 1}, {2, 2}));
    EXPECT_FALSE(is_overlap({2, 2}, {1, 1}));
}

TEST_F(IntervalsTest, GetOverlapSimplTest)
{
    assert((range{2, 2} == get_overlap({1, 2}, {2, 3})));
    assert((range{2, 2} == get_overlap({2, 3}, {1, 2})));
}

TEST_F(IntervalsTest, IntervalSelectionTest)
{
    EXPECT_EQ(0, interval_selection({}));
    EXPECT_EQ(1, interval_selection({{1, 2}}));

    EXPECT_EQ(2, interval_selection({{1, 2}, {1, 2}}));
    EXPECT_EQ(2, interval_selection({{1, 2}, {2, 4}}));
    EXPECT_EQ(2, interval_selection({{2, 4}, {1, 2}}));
    EXPECT_EQ(2, interval_selection({{1, 3}, {4, 6}}));
    EXPECT_EQ(2, interval_selection({{4, 6}, {1, 3}}));

    EXPECT_EQ(2, interval_selection({{1, 2}, {2, 3}, {2, 4}}));
    EXPECT_EQ(2, interval_selection({{1, 2}, {2, 4}, {2, 3}}));
    EXPECT_EQ(2, interval_selection({{2, 3}, {1, 2}, {2, 4}}));
    EXPECT_EQ(2, interval_selection({{2, 3}, {2, 4}, {1, 2}}));
    EXPECT_EQ(2, interval_selection({{2, 4}, {1, 2}, {2, 3}}));
    EXPECT_EQ(2, interval_selection({{2, 4}, {2, 3}, {1, 2}}));

    EXPECT_EQ(2, interval_selection({{1, 5}, {1, 5}, {1, 5}}));

    EXPECT_EQ(3, interval_selection({{1, 3}, {4, 5}, {6, 7}}));
    EXPECT_EQ(3, interval_selection({{1, 3}, {6, 7}, {4, 5}}));
    EXPECT_EQ(3, interval_selection({{4, 5}, {1, 3}, {6, 7}}));
    EXPECT_EQ(3, interval_selection({{4, 5}, {6, 7}, {1, 3}}));
    EXPECT_EQ(3, interval_selection({{6, 7}, {1, 3}, {4, 5}}));
    EXPECT_EQ(3, interval_selection({{6, 7}, {4, 5}, {1, 3}}));

    EXPECT_EQ(4, interval_selection({{1, 10}, {1, 3}, {4, 6}, {7, 10}}));
    EXPECT_EQ(3, interval_selection({{1, 10}, {1, 3}, {3, 6}, {7, 10}}));

    EXPECT_EQ(4, interval_selection({{1, 2}, {2, 3}, {3, 4}, {4, 5}}));
    EXPECT_EQ(2, interval_selection({{1,10}, {2, 9}, {3, 8}, {4, 7}}));
    EXPECT_EQ(2, interval_selection({{1,10}, {2,11}, {3,12}, {4,14}}));
    EXPECT_EQ(3, interval_selection({{1, 5}, {3, 5}, {3, 3}, {4, 4}}));

    EXPECT_EQ(5, interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}}));
    EXPECT_EQ(6, interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}, {1, 5}}));

    // 1234567890
    // ----------
    // ------
    //    ------
    // ---
    //        --
    // -       -
    //  -     -
    EXPECT_EQ(6, interval_selection({{1,10}, {1, 6}, {4,10}, {1, 3}, {8,10}, {1, 1}, {2, 2}, {9, 9}, {10,10}}));

    // 1234567890123456789
    // ----------- --  ---
    // ----------  ---- -- 
    //         ----------
    EXPECT_EQ(6, interval_selection({{1,11}, {1,10}, {9,18}, {13,14}, {17,19}, {13,16}, {18,19}}));

    // 1234567890123456789
    // ------------
    // --------
    //        -----
    //         -
    //           --
    EXPECT_EQ(4, interval_selection({{1,12}, {1, 8}, {8,12}, {9, 9}, {1, 2}}));

    // 1234567890
    // -------
    // -
    //  -
    //   -
    //    -
    //     -
    //   --------
    EXPECT_EQ(6, interval_selection({{1,1}, {2,2}, {3,3}, {4,4}, {5, 5}, {1, 7}, {3, 10}}));
}
