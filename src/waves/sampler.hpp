#pragma once

#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "wave_gen.hpp"


class Renderer
{
public:
    virtual void render(double point) = 0;
    virtual ~Renderer() = default;
};


class OneLineRenderer : public Renderer
{
    double m_min_level;
    double m_max_level;
    std::size_t m_line_len;
    double m_sample_rate;
public:
    OneLineRenderer(std::size_t line_len, double min_level, double max_level)
        : m_line_len(line_len)
    {
        m_sample_rate = (max_level - min_level) / line_len;
        m_min_level = std::min(min_level, max_level);
        m_max_level = std::max(min_level, max_level);
    }

    void render(double point) override
    {
        constexpr char BLOCK = '@';
        std::string line;

        if (m_max_level < point)
            line = std::string (m_line_len, BLOCK);
        else if (m_min_level < point)
        {
            int pos = (point - m_min_level) / m_sample_rate;
            //std::cout << "Point: " << point << " Pos: " << pos << std::endl;
            if (pos > 0)
                line = std::string (pos, BLOCK);
            else
                line = std::string (m_line_len + pos, ' ') + std::string (-pos, BLOCK);
        }

        std::cout << line << std::endl;
    }
};


class Service
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~Service() = default;
};


class Sampler : public Service
{
    const Generator<double>& m_gen;
    Renderer& m_renderer;
    std::chrono::milliseconds m_duration;
    bool m_started = false;
public:
    explicit Sampler(const Generator<double>& gen, Renderer& renderer, std::size_t freq)
        : m_gen(gen), m_renderer(renderer)
    {
        std::chrono::milliseconds sec(1000);
        m_duration = sec / freq;
    }

    void start() override
    {
        if (m_started)
            return;

        m_started = true;
    }

    void stop() override
    {
        if (!m_started)
            return;

        m_started = false;
    }

    void run()
    {
        while (m_started)
        {
            m_renderer.render(m_gen.get());
            std::this_thread::sleep_for(m_duration);
        }
    }
};
