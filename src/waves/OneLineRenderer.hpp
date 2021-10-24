#pragma once

#include "Renderer.hpp"
#include <cstdint>


class OneLineRenderer : public Renderer
{
    double m_min_level;
    double m_max_level;
    std::size_t m_line_len;
    double m_sample_rate;
public:
    OneLineRenderer(std::size_t line_len, double min_level, double max_level);

    void render(double point) override;
};

