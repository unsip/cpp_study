#include "OneLineRenderer.hpp"
#include "sampler.hpp"

int main()
{
    TimePointGen tp_gen;
    SawToothWave saw_gen {WaveArgs::Builder {}.frequency(1.), tp_gen};
    SawToothWave inverse_saw_gen {WaveArgs::Builder {}.frequency(1.), tp_gen};

    SineWave mod_mod_sine {WaveArgs::Builder{}
        .frequency(1)
        .y_offset(1)
        .amp(2)
      , tp_gen
    };
    SineWave mod_sine {WaveArgs::Builder{}
        .frequency(mod_mod_sine)
        .amp(mod_mod_sine)
        .y_offset(1)
        .amp(2)
      , tp_gen
    };
    SineWave sine_gen {WaveArgs::Builder{}.frequency(mod_sine), tp_gen};

    OneLineRenderer term_renderer {100, -1., 1.};
    Sampler sampler {sine_gen, term_renderer, 150};
    sampler.start();
    sampler.run();
}
