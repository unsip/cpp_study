#pragma once

#include "Renderer.hpp"
#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "wave_gen.hpp"


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
