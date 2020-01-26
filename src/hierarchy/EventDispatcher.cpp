#include "EventDispatcher.hpp"

void EventDispatcher::is_dead_emit(Attacker* a_npc, Defender* d_npc, Applier& app_npc) const
{
    for (auto& signal : m_is_dead_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::is_dead_subscribe(signal_type handler)
{
    m_is_dead_signals.push_back(handler);
}

void EventDispatcher::is_dmg_emit(Attacker* a_npc, Defender* d_npc, Applier& app_npc, std::size_t dmg) const
{
    for (auto& signal : m_is_dmg_signals)
        signal(a_npc, d_npc, app_npc, dmg);
}

void EventDispatcher::is_dmg_subscribe(dmg_signal handler)
{
    m_is_dmg_signals.push_back(handler);
}

void EventDispatcher::is_hit_emit(Attacker* a_npc, Defender* d_npc, Applier& app_npc) const
{
    for (auto& signal : m_is_hit_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::is_hit_subscribe(signal_type handler)
{
    m_is_hit_signals.push_back(handler);
}

void EventDispatcher::on_create_emit(Attacker* a_npc, Defender* d_npc, Applier& app_npc, std::string name) const
{
    for (auto& signal : m_on_create_signals)
        signal(a_npc, d_npc, app_npc, name);
}

void EventDispatcher::on_create_subscribe(create_signal handler)
{
    m_on_create_signals.push_back(handler);
}

void EventDispatcher::on_terminate_emit(Attacker* a_npc, Defender* d_npc, Applier& app_npc) const
{
    for (auto& signal : m_on_terminate_signals)
        signal(a_npc, d_npc, app_npc);
}

void EventDispatcher::on_terminate_subscribe(signal_type handler)
{
    m_on_terminate_signals.push_back(handler);
}
