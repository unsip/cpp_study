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
    WaveArgs() = default;

    class Arg
    {
        const Generator<double>* m_gen = nullptr;
        double m_value = 0.;
    public:
        Arg(double v) : m_value(v) {}
        Arg(const Generator<double>& gen) : m_gen(&gen) {}

        double value() const
        {
            return m_gen != nullptr ? m_gen->get() : m_value;
        }
    };

    Arg m_step = Arg {0.};
    Arg m_amp = Arg {1.};
    // Hertz
    Arg m_frequency = Arg {1.};
    Arg m_phase = Arg {0.};
    Arg m_y_offset = Arg {0.};
public:
    class Builder;

    // Wave amplitude size
    double amp() const { return m_amp.value(); }
    // Wave frequency
    double frequency() const { return m_frequency.value(); }
    // Phase offset
    double phase() const { return m_phase.value(); }
    // Amplitude offset
    double y_offset() const { return m_y_offset.value(); }
};


class WaveArgs::Builder
{
    WaveArgs args;
public:
    Builder() = default;
    Builder(const WaveArgs& args) : args(args) {}

    // CV qualifier based overload resolution.
    operator const WaveArgs&() const & { return args; } operator WaveArgs&&() && { return std::move(args); }
    Builder& amp(Arg amp) { this->args.m_amp = amp; return *this; }
    Builder& frequency(Arg frequency) { this->args.m_frequency = frequency; return *this; }
    Builder& phase(Arg phase) { this->args.m_phase = phase; return *this; }
    Builder& y_offset(Arg y_offset) { this->args.m_y_offset = y_offset; return *this; }
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
    mutable double m_curr_freq;
    mutable double m_curr_phase;
    mutable std::chrono::milliseconds m_prev_tp;

public:
    SineWave() = default;
    SineWave(const WaveArgs& args, const Generator<std::chrono::steady_clock::time_point>& gen)
        : m_args(args), m_time_gen(gen), m_curr_freq(args.frequency())
    {
        m_prev_tp = std::chrono::duration_cast<std::chrono::milliseconds>(gen.get().time_since_epoch());
    }

    double get() const override
    {
        auto tp = m_time_gen.get();
        auto ms = m_prev_tp.count();
        m_prev_tp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
        double rad = (ms / 1000. * m_curr_freq) * 2 * std::numbers::pi_v<double>;
        double res = std::sin(rad + m_curr_phase) * m_args.amp() + m_args.y_offset();
        m_curr_freq = m_args.frequency();
        double rad2 = (ms / 1000. * m_curr_freq) * 2 * std::numbers::pi_v<double>;
        m_curr_phase = rad2 - rad;
        assert(rad >= 0.);
        assert(rad2 >= 0.);
        //assert(res + 0.00001 > std::sin(rad2 + m_curr_phase) * m_args.amp() + m_args.y_offset());
        //assert(res - 0.00001 < std::sin(rad2 + m_curr_phase) * m_args.amp() + m_args.y_offset());
        return res;
    }
};

template <typename T>
inline constexpr signed char sgn(const T& value)
{
    return (T{} < value) - (value < T{});
}


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


class Modulator: public Generator<double>
{
    const Generator<double>& m_carrier;
    const Generator<double>& m_modulator;

public:
    Modulator(const Generator<double>& carrier, const Generator<double>& modulator)
        : m_carrier(carrier), m_modulator(modulator)
    {}

    double get() const override
    {
        return m_carrier.get() + m_modulator.get();
    }
};
