#include "EventDispatcher.hpp"

void EventDispatcher::is_dead_emit(Attacker* a_npc, Defender* d_npc, const Applier& app_npc) const
{
    for (auto& signal : m_is_dead_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::is_dead_subscribe(signal_type handler)
{
    m_is_dead_signals.push_back(handler);
}

void EventDispatcher::is_dmg_emit(Attacker* a_npc, Defender* d_npc, const Applier& app_npc) const
{
    for (auto& signal : m_is_dmg_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::is_dmg_subscribe(signal_type handler)
{
    m_is_dmg_signals.push_back(handler);
}

void EventDispatcher::is_hit_emit(Attacker* a_npc, Defender* d_npc, const Applier& app_npc) const
{
    for (auto& signal : m_is_hit_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::is_hit_subscribe(signal_type handler)
{
    m_is_hit_signals.push_back(handler);
}

