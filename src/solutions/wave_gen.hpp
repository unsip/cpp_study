#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <chrono>
#include <numbers>


template <typename T>
class Stream
{
public:
    virtual bool has_next() = 0;
    virtual T get() = 0;
};


template <typename T>
class Generator
{
public:
    virtual T get() const = 0;
    virtual ~Generator() = default;
};


class WaveArgs
{
private:
    double m_step = 0.;
    double m_amp = 1.;
    // Hertz
    double m_frequency = 1.;
    double m_phase = 0.;
    double m_y_offset = 0.;
    WaveArgs() = default;
public:
    class Builder;

    // Discrete step over wave
    double step() const { return m_step; }
    // Wave amplitude size
    double amp() const { return m_amp; }
    // Wave frequency
    double frequency() const { return m_frequency; }
    // Phase offset
    double phase() const { return m_phase; }
    // Amplitude offset
    double y_offset() const { return m_y_offset; }
};


class WaveArgs::Builder
{
    WaveArgs args;
public:
    Builder() = default;
    Builder(const WaveArgs& args) : args(args) {}

    // CV qualifier based overload resolution.
    operator const WaveArgs&() const & { return args; }
    operator WaveArgs&&() && { return std::move(args); }

    Builder& step(double step) { this->args.m_step = step; return *this; }
    Builder& amp(double amp) { this->args.m_amp = amp; return *this; }
    Builder& frequency(double frequency) { this->args.m_frequency = frequency; return *this; }
    Builder& phase(double phase) { this->args.m_phase = phase; return *this; }
    Builder& y_offset(double y_offset) { this->args.m_y_offset = y_offset; return *this; }
};


class TimePointGen: public Generator<std::chrono::steady_clock::time_point>
{
public:
    std::chrono::steady_clock::time_point get() const override
    {
        return std::chrono::steady_clock::now();
    }
};

class SineWave: public Generator<double>
{
    WaveArgs m_args;
    const Generator<std::chrono::steady_clock::time_point>& m_time_gen;

public:
    SineWave() = default;
    SineWave(const WaveArgs& args, const Generator<std::chrono::steady_clock::time_point>& gen)
        : m_args(args), m_time_gen(gen)
    {}

    double get() const override
    {
        auto tp = m_time_gen.get();
        using time_point = std::chrono::steady_clock::time_point;
        using namespace std::chrono;
        std::cout << time_point{nanoseconds{0}}.time_since_epoch().count() << std::endl;
        std::cout << time_point{milliseconds{0}}.time_since_epoch().count() << std::endl;

        std::cout << "Since epoch: " << tp.time_since_epoch().count() << std::endl;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        double rad = (ms % 1000 / 1000. * m_args.frequency()) * 2 * std::numbers::pi_v<double>;
        std::cout << "Ms: " << ms << std::endl;
        std::cout << "Rad: " << rad << std::endl;
        return std::sin(rad + m_args.phase()) * m_args.amp() + m_args.y_offset();
    }
};
