#include "bestiary.hpp"

#include <algorithm>
#include <limits>


void Rat::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

void Hulk::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    m_strength = std::numeric_limits<std::size_t>::max() / 2 <= m_strength
        ? m_strength * 2
        : std::numeric_limits<std::size_t>::max();
}

void Mimic::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
    m_strength = m_hp;
}

void Slime::hit(std::size_t dmg)
{
    m_hp += std::min(dmg, std::numeric_limits<std::size_t>::max() - m_hp);
}

void Door::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}
