#pragma once

#include <cmath>
#include <cassert>
#include <chrono>
#include <numbers>


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
    operator const WaveArgs&() const & { return args; } operator WaveArgs&&() && { return std::move(args); } 
    Builder& step(double step) { this->args.m_step = step; return *this; }
    Builder& amp(double amp) { this->args.m_amp = amp; return *this; }
    Builder& frequency(double frequency) { this->args.m_frequency = frequency; return *this; }
    Builder& phase(double phase) { this->args.m_phase = phase; return *this; }
    Builder& y_offset(double y_offset) { this->args.m_y_offset = y_offset; return *this; }
};


class TimePointGen: public Generator<std::chrono::steady_clock::time_point>
{ public:
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
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        double rad = (ms / 1000. * m_args.frequency()) * 2 * std::numbers::pi_v<double>;
        assert(rad >= 0.);
        return std::sin(rad + m_args.phase()) * m_args.amp() + m_args.y_offset();
    }
};

template <typename T>
inline constexpr signed char sgn(const T& value)
{
    return (T{} < value) - (value < T{});
}

//class SquareWave : public Generator<double>
//{
//    const SineWave& m_sine_gen;
//
//public:
//    SquareWave(const SineWave& gen)
//        : m_sine_gen(gen)
//    {}
//
//    double get() const override
//    {
//        return sgn(m_sine_gen.get());
//    }
//};


class SawToothWave: public Generator<double>
{
    WaveArgs m_args;
    const Generator<std::chrono::steady_clock::time_point>& m_time_gen;

public:
    SawToothWave(const WaveArgs& args, const Generator<std::chrono::steady_clock::time_point>& gen)
        : m_args(args), m_time_gen(gen)
    {}

    double get() const override
    {
        auto tp = m_time_gen.get();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        return -2. * m_args.amp() / std::numbers::pi_v<double> * std::atan(
            1. / std::tan(ms / 1000. * m_args.frequency() * std::numbers::pi_v<double> + m_args.phase())
        );
    }
};


class RingModulator : public Generator<double>
{
    const Generator<double>& m_carrier;
    const Generator<double>& m_modulator;

public:
    RingModulator(const Generator<double>& carrier, const Generator<double>& modulator)
        : m_carrier(carrier), m_modulator(modulator)
    {}

    double get() const override
    {
        return m_carrier.get() + m_modulator.get();
    }
};
