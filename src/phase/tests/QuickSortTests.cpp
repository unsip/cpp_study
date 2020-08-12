#include <gtest/gtest.h>

#include "QuickSort.hpp"

#include <algorithm>
#include <vector>


class QuickSortTests : public ::testing::Test
{
public:
};

TEST_F(QuickSortTests, EmptyTest)
{
    std::vector<int> arr;
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, std::vector<int> {});
}

TEST_F(QuickSortTests, SingleElemTest)
{
    std::vector<double> arr = {9};
    std::vector<double> res = {9};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, TwoElemsTest)
{
    std::vector<double> arr = {2, 1};
    std::vector<double> res = {1, 2};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, ThreeElemsTest)
{
    std::vector<double> arr = {9, 6, 1};
    std::vector<double> res = {1, 6, 9};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, EqualElemsTest)
{
    std::vector<int> arr = {1, 1, 1, 1, 1};
    std::vector<int> res = {1, 1, 1, 1, 1};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, DuplicatesTest)
{
    std::vector<int> arr = {3, 1, 2, 1, 2, 3};
    std::vector<int> res = {1, 1, 2, 2, 3, 3};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, SortedSeqTest)
{
    std::vector<int> arr = {1, 2, 3, 4};
    std::vector<int> res = {1, 2, 3, 4};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, SimpleDuplicatesSeqTest)
{
    std::vector<int> arr = {1, 4, 3, 4};
    std::vector<int> res = {1, 3, 4, 4};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, ReversedTest)
{
    std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::vector<int> res = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

TEST_F(QuickSortTests, NegativeIntsTest)
{
    std::vector<int> arr = {
        9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 111, 1314, 5155, 5123, 555, 5111, 999,
        313, 31241, 4124, 41425, 512414, 313, 3213, 425, 52523, 523423, 62325,
        42432, 5299, 2834, -123, -315151, 5123, 5, 0, 31, -1, 3213, 425,
        -52523, 523423, -62325, 42432, 5299, 2834, 123, -315151, 5123, 5, 0,
        31, -1
    };
    std::vector<int> res = {
        9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 111, 1314, 5155, 5123, 555, 5111, 999,
        313, 31241, 4124, 41425, 512414, 313, 3213, 425, 52523, 523423, 62325,
        42432, 5299, 2834, -123, -315151, 5123, 5, 0, 31, -1, 3213, 425,
        -52523, 523423, -62325, 42432, 5299, 2834, 123, -315151, 5123, 5, 0,
        31, -1
    };
    std::sort(res.begin(), res.end());
    phase::algorithms::quick_sort(arr.begin(), arr.end());
    EXPECT_EQ(arr, res);
}

