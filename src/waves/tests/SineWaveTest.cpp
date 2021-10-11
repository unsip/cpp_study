#include <vector>
#include <ranges>
#include <gtest/gtest.h>
#include "../wave_gen.hpp"


class SineWaveTest : public ::testing::Test
{
public:
};

template <std::forward_iterator FwIt>
class TimePointMock : public Generator<std::chrono::steady_clock::time_point>
{
    mutable FwIt m_begin;
    FwIt m_end;
public:
    template <std::ranges::forward_range Rng>
    TimePointMock(const Rng& rng) : m_begin(begin(rng)), m_end(end(rng))
    {}

    std::chrono::steady_clock::time_point get() const override
    {
        assert(m_begin != m_end && "Fucked up iters!");
        return *m_begin++;
    }
};

template <std::ranges::forward_range Rng>
TimePointMock(const Rng&) -> TimePointMock<decltype(begin(std::declval<Rng>()))>;


TEST_F(SineWaveTest, SimpleWaveTest)
{
    std::vector tps = {
        std::chrono::steady_clock::time_point {std::chrono::milliseconds {0}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {0}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {125}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {250}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {375}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {500}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {750}}
      , std::chrono::steady_clock::time_point {std::chrono::milliseconds {1000}}
    };
    TimePointMock gen {tps};
    SineWave wave_gen {WaveArgs::Builder{}, gen};


    /// @todo an accuracy isn't enough
    constexpr auto DELTA = .001;
    EXPECT_NEAR(0.000, wave_gen.get(), DELTA);
    EXPECT_NEAR(0.707, wave_gen.get(), DELTA);
    EXPECT_NEAR(1., wave_gen.get(), DELTA);
    EXPECT_NEAR(0.707, wave_gen.get(), DELTA);
    EXPECT_NEAR(0., wave_gen.get(), DELTA);
    EXPECT_NEAR(-1., wave_gen.get(), DELTA);
    EXPECT_NEAR(0., wave_gen.get(), DELTA);
}

