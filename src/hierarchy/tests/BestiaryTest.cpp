#include <gtest/gtest.h>
#include "../Bestiary.hpp"
#include "../EventDispatcher.hpp"


class BestiaryTest : public ::testing::Test
{
public:
    EventDispatcher ed;
};

TEST_F(BestiaryTest, SimpleRatTest)
{
    Rat r{10, 1, ed};
    r.hit(9);

    EXPECT_EQ(1, r.get_hp());
    EXPECT_EQ(1, r.attack());
    EXPECT_FALSE(r.is_dead());
}

TEST_F(BestiaryTest, SimplePlagueRatTest)
{
    PlagueRat r{10, 1, 1, ed};
    r.hit(0);

    EXPECT_EQ(9, r.get_hp());
    EXPECT_EQ(2, r.attack());
    EXPECT_FALSE(r.is_dead());
}

TEST_F(BestiaryTest, SimpleArmoredRatTest)
{
    ArmoredRat r{10, 1, 2, ed};

    r.hit(0);
    EXPECT_EQ(10, r.get_hp());

    r.hit(10);
    EXPECT_EQ(2, r.get_hp());
    EXPECT_FALSE(r.is_dead());

    r.hit(2);
    EXPECT_EQ(2, r.get_hp());
    EXPECT_FALSE(r.is_dead());

    EXPECT_EQ(1, r.attack());
}

TEST_F(BestiaryTest, PlagueRatDeathTest)
{
    PlagueRat r1{10, 1, 1, ed};
    r1.hit(9);
    EXPECT_TRUE(r1.is_dead());
    EXPECT_EQ(0, r1.get_hp());
    EXPECT_EQ(1, r1.get_rot_stack());

    r1.hit(99999999999);
    EXPECT_TRUE(r1.is_dead());
    EXPECT_EQ(0, r1.get_hp());
    EXPECT_EQ(2, r1.get_rot_stack());
}

bool kill_rat(IRat& r)
{
    r.hit(r.get_hp());

    return r.is_dead();
}

TEST_F(BestiaryTest, RatsIsDeadTest)
{
    PlagueRat r1{10, 1, 1, ed};
    Rat r2 {12, 4, ed};
    ArmoredRat r3 {42245, 39, 23, ed};

    EXPECT_TRUE(kill_rat(r1));
    EXPECT_TRUE(kill_rat(r2));
    EXPECT_FALSE(kill_rat(r3));
}
