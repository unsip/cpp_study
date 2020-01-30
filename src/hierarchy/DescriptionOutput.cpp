#include "DescriptionOutput.hpp"

#include <iostream>
#include <cassert>

void DescriptionOutput::visit(Attacker*, Defender*, Applier&) const
{
    assert(!"Can't reach here!");
}

void DescriptionOutput::visit(IRat&) const
{
    assert(!"Can't reach here!");
}

void DescriptionOutput::visit(Rat&) const
{
    std::cout << "Simple rat. Nothing special." << std::endl;
}

void DescriptionOutput::visit(ArmoredRat&) const
{
    std::cout << "Armored rat has armor, which diminishes incoming damage." << std::endl;
}

void DescriptionOutput::visit(PlagueRat&) const
{
    std::cout << "Plague rat rots on each incoming hit, which increases it's attack." << std::endl;
}

void DescriptionOutput::visit(Hulk&) const
{
    std::cout <<
        "Hulk is very aggressive and tough. "
        "His attack is multiplied by 2 on each incoming hit."
        << std::endl;
}

void DescriptionOutput::visit(Mimic&) const
{
    std::cout << "Mimic always has it's attack equal to health" << std::endl;
}


void DescriptionOutput::visit(ZombieMimic&) const
{
    std::cout << "Zombie Mimic reborns after death." << std::endl;
}

void DescriptionOutput::visit(Slime&) const
{
    std::cout << "Regular slime, but gets healed on ammount of incoming damage." << std::endl;
}

void DescriptionOutput::visit(SlimeShard&) const
{
    std::cout <<
        "Slime shard was a part of something BIGGER once. "
        "Now it's just a weak and fragile creature."
        << std::endl;
}

void DescriptionOutput::visit(SlimeQueen&) const
{
    std::cout <<
        "Mother of slimes. "
        "Very dangerous, might be divided into few slime shards. "
        "Or even something BIGGER...should be killed in a few hits as possible."
        << std::endl;
}

void DescriptionOutput::visit(PoisonCloud&) const
{
    std::cout <<
        "Choking gas floating in space. "
        "The only way to defeat it is to avoid."
        << std::endl;
}

void DescriptionOutput::visit(Door&) const
{
    std::cout << "Just a solid door. What does lay behind?" << std::endl;
}

void DescriptionOutput::visit(StonePortal&) const
{
    std::cout <<
        "Ancient portal leading to distant mysterious places. "
        "Absorbs incoming damage due to it's heavy construction."
        << std::endl;
}
