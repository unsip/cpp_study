#include "EventDecorator.hpp"

bool EventDecorator::is_dead() const
{
    return m_def_npc->is_dead();
}

std::size_t EventDecorator::get_hp() const
{
    return m_def_npc->get_hp();
}

void EventDecorator::hit(std::size_t dmg)
{
    auto hp_before_hit = get_hp();

    m_ed.is_hit_emit(m_att_npc, m_def_npc, m_app_npc);

    m_def_npc->hit(dmg);

    if (get_hp() < hp_before_hit)
    {
        m_ed.is_dmg_emit(m_att_npc, m_def_npc, m_app_npc, dmg);
        if (is_dead())
            m_ed.is_dead_emit(m_att_npc, m_def_npc, m_app_npc);
    }
}
