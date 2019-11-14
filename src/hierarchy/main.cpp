#include "BestiaryFactory.hpp"
#include "EventDispatcher.hpp"

#include <iostream>
#include <iomanip>

int main()
{
    EventDispatcher ed;
    BestiaryFactory monster_gen{10, ed};

    for (std::size_t i = 0; i < 50; ++i)
    {
        if (monster_gen.is_last_man_standing())
        {
            auto a = monster_gen.get_rnd_attacker();
            std::cout
                << "  ==================================\n"
                << "  And the winner is " << a.first << "!!!\n"
                << "  ==================================\n";
            std::cout.flush();
            break;
        }

        // TODO: Refactor dirty way of getting defenders.
        decltype(monster_gen.get_rnd_attacker()) a;
        decltype(monster_gen.get_rnd_defender()) d;
        do
        {
            a = monster_gen.get_rnd_attacker();
            d = monster_gen.get_rnd_defender();
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
