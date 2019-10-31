#include "DescriptionOutput.hpp"

#include <iostream>

void DescriptionOutput::visit(const Rat& rat) const
{
    std::cout << "Simple rat. Nothing special." << std::endl;
}

void DescriptionOutput::visit(const ArmoredRat& rat) const
{
    std::cout << "Armored rat has armor, which diminishes incoming damage." << std::endl;
}

void DescriptionOutput::visit(const PlagueRat& rat) const
{
    std::cout << "Plague rat rots on each incoming hit, which increases it's attack." << std::endl;
}

void DescriptionOutput::visit(const Hulk& hulk) const
{
    std::cout <<
        "Hulk is very aggressive and tough. "
        "His attack is multiplied by 2 on each incoming hit."
        << std::endl;
}

void DescriptionOutput::visit(const Mimic& mimic) const
{
    std::cout << "Mimic always has it's attack equal to health" << std::endl;
}

void DescriptionOutput::visit(const Slime& slime) const
{
    std::cout << "Regular slime, but gets healed on ammount of incoming damage." << std::endl;
}

void DescriptionOutput::visit(const SlimeShard& slime) const
{
    std::cout <<
        "Slime shard was a part of something BIGGER once. "
        "Now it's just a weak and fragile creature."
        << std::endl;
}

void DescriptionOutput::visit(const SlimeQueen& slime) const
{
    std::cout <<
        "Mother of slimes. "
        "Very dangerous, might be divided into few slime shards. "
        "Or even something BIGGER...should be killed in a few hits as possible."
        << std::endl;
}

void DescriptionOutput::visit(const PoisonCloud& cloud) const
{
    std::cout <<
        "Choking gas floating in space. "
        "The only way to defeat it is to avoid."
        << std::endl;
}

void DescriptionOutput::visit(const Door& door) const
{
    std::cout << "Just a solid door. What does lay behind?" << std::endl;
}

void DescriptionOutput::visit(const StonePortal& portal) const
{
    std::cout <<
        "Ancient portal leading to distant mysterious places. "
        "Absorbs incoming damage due to it's heavy construction."
        << std::endl;
}
