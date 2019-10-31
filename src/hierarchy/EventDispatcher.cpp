#include "EventDispatcher.hpp"

void EventDispatcher::is_dead_emit(Attacker* a_npc, Defender* d_npc) const
{
    for (auto& signal : m_is_dead_signals)
        signal(a_npc, d_npc);
}

void EventDispatcher::is_dead_subscribe(signal_type handler)
{
    m_is_dead_signals.push_back(handler);
}
