#include "factory.hpp"

#include <iostream>

void attack(const Attacker& attacker, Defender& defender)
{
    defender.hit(attacker.attack());
}

int main()
{
    BestiaryFactory monster_gen{10};

    for (std::size_t i = 0; i < 50; ++i)
    {
        auto a = monster_gen.get_rnd_attacker();
        auto d = monster_gen.get_rnd_defender();

        while (a.first == d.second)
            d = monster_gen.get_rnd_defender();

        attack(*a.second, *d.second);
    }

    return 0;
}
