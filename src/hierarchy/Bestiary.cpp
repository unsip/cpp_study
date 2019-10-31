#include "IBestiaryVisitor.hpp"
#include "Bestiary.hpp"

#include <algorithm>
#include <limits>


void Rat::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(this, this, *this);

    m_hp -= std::min(m_hp, dmg);

    if (dmg)
        m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void Rat::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void ArmoredRat::hit(std::size_t dmg)
{
    std::size_t admg = dmg < m_armor ? 0 : dmg - m_armor;
    Rat::hit(admg);
}

void ArmoredRat::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void PlagueRat::hit(std::size_t dmg)
{
    // TODO: Might need to emit hit before rot stacks.
    m_rot_stack += m_rot;
    Rat::hit(dmg + m_rot);
}

void PlagueRat::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void Hulk::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(this, this, *this);

    m_hp -= std::min(m_hp, dmg);

    m_strength = m_strength <= std::numeric_limits<std::size_t>::max() / 2
        ? m_strength * 2
        : std::numeric_limits<std::size_t>::max();

    if (dmg)
        m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void Hulk::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void Mimic::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(this, this, *this);

    m_hp -= std::min(m_hp, dmg);
    m_strength = m_hp;

    if (dmg)
        m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void Mimic::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void Slime::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(this, this, *this);

    m_hp += std::min(dmg, std::numeric_limits<std::size_t>::max() - m_hp);

    if (dmg)
        m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void Slime::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void SlimeShard::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(this, this, *this);

    m_hp -= std::min(m_hp, dmg);

    if (dmg)
        m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void SlimeShard::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void SlimeQueen::hit(std::size_t dmg)
{
//    m_hp -= std::min(m_hp, dmg);
//
//    if (is_dead())
//        m_ed.is_dead_emit(this, this);
//    else
//    {
//        // TODO
//        // Roll 1d6
//        // if 6 then m_ed.emit_queen_spawn
//        // else
//        //   1d2 % 2 ? m_ed.emit_slime_shard_spawn : pass
//        //
//    }
}

void SlimeQueen::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void Door::hit(std::size_t dmg)
{
    m_ed.is_hit_emit(nullptr, this, *this);

    m_hp -= std::min(m_hp, dmg);

    if (dmg)
        m_ed.is_dmg_emit(nullptr, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(nullptr, this, *this);
}

void Door::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void StonePortal::hit(std::size_t dmg)
{
    Door::hit(dmg / 2);
}

void StonePortal::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}

void PoisonCloud::apply(const IBestiaryVisitor& v) const
{
    v.visit(*this);
}
