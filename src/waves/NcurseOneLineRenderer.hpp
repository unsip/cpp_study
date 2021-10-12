#pragma once

#include "Renderer.hpp"
#include <chrono>
#include <optional>
#include <memory>

class NcurseOneLineRenderer : public Renderer
{
    class Impl;
    // income signal level to represent one vertical point
    // can be negative if original max lvl less than min
    std::unique_ptr<Impl> m_impl;
    double m_sample_rate;
    double m_min_level;
    double m_max_level;
    std::chrono::steady_clock::time_point m_prev_tp;
    std::optional<double> m_prev_lvl;

    /// signal level in terms of graph bar size
    float level_to_bar(float lvl) const;

public:
    NcurseOneLineRenderer(double min_level, double max_level);
    ~NcurseOneLineRenderer();

    void render(double point) override;
};

