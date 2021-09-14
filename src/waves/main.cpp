#include "sampler.hpp"

int main()
{
    TimePointGen tp_gen;
    SawToothWave saw_gen {WaveArgs::Builder {}.frequency(1.).phase(3.14 / 2), tp_gen};
    SawToothWave inverse_saw_gen {WaveArgs::Builder {}.frequency(1.).amp(-1.), tp_gen};
    SineWave sine_gen {WaveArgs::Builder {}.frequency(0.5), tp_gen};
    //SquareWave square_gen {sine_gen};
    RingModulator ring {saw_gen, inverse_saw_gen};
    OneLineRenderer term_renderer {50, -3., 3.};
    Sampler sampler {inverse_saw_gen, term_renderer, 50};
    sampler.start();
    sampler.run();
}
