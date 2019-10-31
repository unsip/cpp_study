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
        auto a = monster_gen.get_rnd_attacker();
        auto d = monster_gen.get_rnd_defender();

        while (a.first == d.first)
            d = monster_gen.get_rnd_defender();

        auto dmg = a.second->attack();

        using namespace std;
        cout << setw(15) << a.first << " attacks (" << setw(3) << dmg << ") "
            << setw(20) << d.first << " (" << setw(2) << d.second->get_hp() << " hp";
        d.second->hit(dmg);
        cout << " --> " << setw(3) << d.second->get_hp() << " hp)" << endl;
    }

    return 0;
}
