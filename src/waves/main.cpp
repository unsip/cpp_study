#include "NcurseOneLineRenderer.hpp"
#include "OneLineRenderer.hpp"
#include "sampler.hpp"


class DummyRenderer: public Renderer
{
public:
    void render(double point) override
    {
        std::cout << "Point: " << point << std::endl;
    }
};


int main()
{
    TimePointGen tp_gen;
    SineWave sine_wave_gen {WaveArgs::Builder {}.frequency(5.), tp_gen};
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
    SineWave sine_gen {WaveArgs::Builder{}.frequency(mod_mod_sine), tp_gen};
    //SawToothWave saw_gen {WaveArgs::Builder{}
    //    .frequency(mod_mod_sine)
    //    .amp(mod_mod_sine)
    //    .y_offset(1)
    //    .amp(2)
    //  , tp_gen
    //};

    std::cout.sync_with_stdio(false);
    setlocale(LC_ALL, "");
    //OneLineRenderer term_renderer {150, -1., 1.};
    NcurseOneLineRenderer term_renderer {-1., 1.};
    DummyRenderer dummy_renderer {};
    Sampler sampler {sine_wave_gen, term_renderer, 100};
    sampler.start();
    sampler.run();
}
