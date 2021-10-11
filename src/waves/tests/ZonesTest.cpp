#include <gtest/gtest.h>
#include "../Zones.h"
#include <limits>

class ZonesTest : public ::testing::Test
{
public:
    static constexpr auto err = std::numeric_limits<float>::round_error();
};

TEST_F(ZonesTest, SimpleZonesTest1)
{
    Zones z{1, 1};
    EXPECT_EQ(1, z.size());

    EXPECT_EQ(0, z.idx_by_lvl(z.min()));
    EXPECT_EQ(0, z.idx_by_lvl(z.max() - err));

    EXPECT_EQ(0, z.idx_by_lvl(0));
    EXPECT_EQ(0, z.idx_by_lvl(.5));
    EXPECT_EQ(0, z.idx_by_lvl(1 - err));

    EXPECT_EQ(std::make_pair(0.f, 1.f), z.at(0));
}

TEST_F(ZonesTest, SimpleZonesTest2)
{
    Zones z{1, 2};
    EXPECT_EQ(2, z.size());

    EXPECT_EQ(0, z.idx_by_lvl(z.min()));
    EXPECT_EQ(z.size() - 1, z.idx_by_lvl(z.max() - err));

    EXPECT_EQ(0, z.idx_by_lvl(0));
    EXPECT_EQ(0, z.idx_by_lvl(z.delta() - err));
    EXPECT_EQ(1, z.idx_by_lvl(z.delta()));
    EXPECT_EQ(1, z.idx_by_lvl(2 * z.delta() - err));

    EXPECT_EQ(std::make_pair(z.min(), z.delta()), z.at(0));
    EXPECT_EQ(std::make_pair(z.delta(), z.max()), z.at(1));
}

TEST_F(ZonesTest, SimpleZonesTest3)
{
    Zones z{10, 3};
    EXPECT_EQ(3, z.size());

    EXPECT_EQ(0, z.idx_by_lvl(z.min()));
    EXPECT_EQ(z.size() - 1, z.idx_by_lvl(z.max() - err));

    EXPECT_EQ(0, z.idx_by_lvl(0));
    EXPECT_EQ(0, z.idx_by_lvl(1));
    EXPECT_EQ(0, z.idx_by_lvl(2));
    EXPECT_EQ(0, z.idx_by_lvl(3));
    EXPECT_EQ(0, z.idx_by_lvl(z.delta() - err));
    EXPECT_EQ(1, z.idx_by_lvl(z.delta()));
    EXPECT_EQ(1, z.idx_by_lvl(4));
    EXPECT_EQ(1, z.idx_by_lvl(5));
    EXPECT_EQ(1, z.idx_by_lvl(6));
    EXPECT_EQ(1, z.idx_by_lvl(z.delta() * 2 - err));
    EXPECT_EQ(2, z.idx_by_lvl(z.delta() * 2));
    EXPECT_EQ(2, z.idx_by_lvl(7));
    EXPECT_EQ(2, z.idx_by_lvl(8));
    EXPECT_EQ(2, z.idx_by_lvl(9));
    EXPECT_EQ(2, z.idx_by_lvl(z.max() - err));

    EXPECT_EQ(std::make_pair(z.min(), z.delta()), z.at(0));
    EXPECT_EQ(std::make_pair(z.delta(), 2 * z.delta()), z.at(1));
    EXPECT_EQ(std::make_pair(2 * z.delta(), z.max()), z.at(2));
}
