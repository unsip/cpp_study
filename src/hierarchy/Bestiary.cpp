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

void Hulk::apply(const IBestiaryVisitor& v)
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

void Mimic::apply(const IBestiaryVisitor& v)
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

void Slime::apply(const IBestiaryVisitor& v)
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

void SlimeShard::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void SlimeQueen::hit(std::size_t dmg)
{
    if (is_dead())
        return;

    m_ed.is_hit_emit(this, this, *this);
    if (0 == dmg)
        return;

    if (dmg >= m_hp)
        spawn_shards();

    m_hp -= std::min(m_hp, dmg);
    m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
}

void SlimeQueen::apply(const IBestiaryVisitor& v)
{
    v.visit(*this);
}

void SlimeQueen::spawn_shards()
{
    std::size_t total = get_hp() / 2;
    m_brood.reserve(total);
    for (std::size_t i = 0; i < total; ++i)
        m_brood.emplace_back(new SlimeShard(2, m_ed));
}

std::vector<SlimeShard*> SlimeQueen::detach_shards()
{
    return std::move(m_brood);
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
    if (is_dead())
        return;

    m_ed.is_hit_emit(this, this, *this);
    if (0 == dmg)
        return;

    m_hp -= std::min(m_hp, dmg);
    m_ed.is_dmg_emit(this, this, *this);

    if (is_dead())
        m_ed.is_dead_emit(this, this, *this);
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
