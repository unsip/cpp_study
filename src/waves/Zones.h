#pragma once

#include <algorithm>
#include <limits>
#include <cassert>
#include <cstddef>

class Zones
{
    const float m_height;
    const std::size_t m_num;

public:
    constexpr Zones(std::size_t height, std::size_t num)
      : m_height{static_cast<float>(height)}, m_num{num}
    {
        assert(height > 0);
        assert(num > 0);
        assert(height <= std::numeric_limits<decltype(m_height)>::max());
    }

    constexpr std::size_t size() const { return m_num; }
    constexpr float min() const { return .0; }
    constexpr float max() const { return m_height; }
    constexpr float delta() const { return max() / size(); }

    constexpr std::size_t idx_by_lvl(float lvl) const
    {
        assert(lvl >= min());
        assert(lvl <= max());
        return std::min<std::size_t>(lvl / delta(), size() - 1);
    }

    constexpr std::pair<float, float> at(std::size_t idx) const
    {
        assert(idx < size());
        const auto lo = idx * delta();
        const auto hi = lo + delta();
        return {lo, std::min(hi, max())};
    }
};

