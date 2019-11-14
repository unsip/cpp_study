#include "BestiaryFactory.hpp"
#include "Scene.hpp"
#include "EventDispatcher.hpp"

#include <iostream>
#include <iomanip>

int main()
{
    EventDispatcher ed;
    BestiaryFactory monster_gen{ed};
    Scene scene{10, monster_gen, ed};

    for (std::size_t i = 0; i < 50; ++i)
    {
        if (scene.is_last_man_standing())
        {
            auto a = scene.get_rnd_attacker();
            std::cout
                << "  ==================================\n"
                << "  And the winner is " << a.first << "!!!\n"
                << "  ==================================\n";
            std::cout.flush();
            break;
        }

        // TODO: Refactor dirty way of getting defenders.
        decltype(scene.get_rnd_attacker()) a;
        decltype(scene.get_rnd_defender()) d;
        do
        {
            a = scene.get_rnd_attacker();
            d = scene.get_rnd_defender();
        }
        while (a.first == d.first);

        auto dmg = a.second->attack();

        using namespace std;
        cout << setw(15) << a.first << " attacks (" << setw(3) << dmg << ") "
            << setw(20) << d.first << " (" << setw(2) << d.second->get_hp() << " hp";
        d.second->hit(dmg);
        cout << " --> " << setw(3) << d.second->get_hp() << " hp)" << endl;
    }

    return 0;
}
