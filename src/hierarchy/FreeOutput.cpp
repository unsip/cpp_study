#include "FreeOutput.hpp"

#include <iomanip>
#include <iostream>
#include <cassert>

namespace
{
template <typename T>
void print_basic_stats(T& npc)
{
    std::cout
        << "\nHealth: " << npc.get_hp()
        << "\nAttack: " << npc.attack()
        << "\nDead: " << std::boolalpha << npc.is_dead();
}
}

void FreeOutput::visit(Attacker*, Defender*, Applier&) const
{
    assert(!"Can't reach here!");
}

void FreeOutput::visit(Rat& rat) const
{
    std::cout << "Class: Rat";
    print_basic_stats(rat);
    std::cout << std::endl;
}

void FreeOutput::visit(ArmoredRat& rat) const
{
    std::cout << "Class: Armored Rat";
    print_basic_stats(rat);
    std::cout << "\nArmor: " << rat.get_armor() << std::endl;
}

void FreeOutput::visit(PlagueRat& rat) const
{
    std::cout << "Class: Plague Rat";
    print_basic_stats(rat);
    std::cout
        << "\nRot over time: " << rat.get_rot()
        << "\nRot stack: " << rat.get_rot_stack()
        << std::endl;
}

void FreeOutput::visit(Hulk& hulk) const
{
    std::cout << "Class: Hulk";
    print_basic_stats(hulk);
    std::cout << std::endl;
}

void FreeOutput::visit(Mimic& mimic) const
{
    std::cout << "Class: Mimic";
    print_basic_stats(mimic);
    std::cout << std::endl;
}

void FreeOutput::visit(ZombieMimic& zmimic) const
{
    std::cout << "Class: Zombie Mimic";
    print_basic_stats(zmimic);
    std::cout << std::endl;
}

void FreeOutput::visit(Slime& slime) const
{
    std::cout << "Class: Slime";
    print_basic_stats(slime);
    std::cout << std::endl;
}

void FreeOutput::visit(SlimeShard& slime) const
{
    std::cout << "Class: Slime Shard";
    print_basic_stats(slime);
    std::cout << std::endl;
}

void FreeOutput::visit(SlimeQueen& slime) const
{
    std::cout << "Class: Slime Queen";
    print_basic_stats(slime);
    std::cout << std::endl;
}

void FreeOutput::visit(PoisonCloud& cloud) const
{
    std::cout
        << "Class: Poison Cloud"
        << "\nAttack: " << cloud.attack()
        << std::endl;
}

void FreeOutput::visit(Door& door) const
{
    std::cout
        << "Class: Door"
        << "\nHealth: " << door.get_hp()
        << "\nDead: " << std::boolalpha << door.is_dead()
        << std::endl;
}

void FreeOutput::visit(StonePortal& portal) const
{
    std::cout
        << "Class: Stone Portal"
        << "\nHealth: " << portal.get_hp()
        << "\nDead: " << std::boolalpha << portal.is_dead()
        << std::endl;
}
