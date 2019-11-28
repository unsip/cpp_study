#include "IBestiaryVisitor.hpp"
#include "Bestiary.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cassert>


void Rat::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

void Rat::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void ArmoredRat::hit(std::size_t dmg)
{
    std::size_t admg = dmg < m_armor ? 0 : dmg - m_armor;
    Rat::hit(admg);
}

void ArmoredRat::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void PlagueRat::hit(std::size_t dmg)
{
    // TODO: Might need to emit hit before rot stacks.
    m_rot_stack += m_rot;
    Rat::hit(dmg + m_rot);
}

void PlagueRat::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void Hulk::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);

    m_strength = m_strength <= std::numeric_limits<std::size_t>::max() / 2
        ? m_strength * 2
        : std::numeric_limits<std::size_t>::max();
}

void Hulk::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void Mimic::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
    m_strength = m_hp;
}

void Mimic::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void Slime::hit(std::size_t dmg)
{
    if (!is_dead())
        m_hp += std::min(dmg, std::numeric_limits<std::size_t>::max() - m_hp);
}

void Slime::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void SlimeShard::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

void SlimeShard::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void SlimeQueen::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

void SlimeQueen::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void SlimeQueen::spawn_shards(std::size_t before_death_hp)
{
    assert(before_death_hp && "0 isn't damage!");
    std::size_t total = before_death_hp / 2;
    m_brood.reserve(total);
    for (std::size_t i = 0; i < total; ++i)
        m_brood.emplace_back(new SlimeShard(2));
}

std::vector<SlimeShard*> SlimeQueen::detach_shards()
{
    return std::move(m_brood);
}

void Door::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

void Door::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void StonePortal::hit(std::size_t dmg)
{
    Door::hit(dmg / 2);
}

void StonePortal::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void PoisonCloud::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void ZombieMimic::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void ZombieMimic::hit(std::size_t dmg)
{
    m_hp -= std::min(m_hp, dmg);
}

bool ZombieMimic::reborn()
{
    if (!is_dead())
        throw std::runtime_error("Can't reborn alive ZombieMimic!");

    if (m_reborn_cnt)
    {
        --m_reborn_cnt;
        m_hp = m_reborn_hp;
        m_reborn_hp /= 2;
        return true;
    }

    return false;
}
