#include "Bestiary.hpp"

#include <algorithm>
#include <limits>


void Rat::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    if (is_dead())
        m_ed.is_dead_emit(this, this);
}

void ArmoredRat::hit(std::size_t dmg)
{
    std::size_t admg = dmg < m_armor ? 0 : dmg - m_armor;
    Rat::hit(admg);
}

void PlagueRat::hit(std::size_t dmg)
{
    m_rot_stack += m_rot;
    Rat::hit(dmg + m_rot);
}

void Hulk::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    m_strength = m_strength <= std::numeric_limits<std::size_t>::max() / 2
        ? m_strength * 2
        : std::numeric_limits<std::size_t>::max();

    if (is_dead())
        m_ed.is_dead_emit(this, this);
}

void Mimic::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
    m_strength = m_hp;

    if (is_dead())
        m_ed.is_dead_emit(this, this);
}

void Slime::hit(std::size_t dmg)
{
    m_hp += std::min(dmg, std::numeric_limits<std::size_t>::max() - m_hp);

    if (is_dead())
        m_ed.is_dead_emit(this, this);
}

void SlimeShard::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    if (is_dead())
        m_ed.is_dead_emit(this, this);
}

void SlimeQueen::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    if (is_dead())
        m_ed.is_dead_emit(this, this);
    else
    {
        // Roll 1d6
        // if 6 then m_ed.emit_queen_spawn
        // else
        //   1d2 % 2 ? m_ed.emit_slime_shard_spawn : pass
        //
    }
}

void Door::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    if (is_dead())
        m_ed.is_dead_emit(nullptr, this);
}

void StonePortal::hit(std::size_t dmg)
{
    Door::hit(dmg / 2);
}
