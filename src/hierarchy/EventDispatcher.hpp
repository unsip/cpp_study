#pragma once

#include "IEventDispatcher.hpp"

#include <vector>


class EventDispatcher : public IEventDispatcher
{
private:
    std::vector<signal_type> m_is_dead_signals;
    std::vector<signal_type> m_is_dmg_signals;
    std::vector<signal_type> m_is_hit_signals;

    std::vector<signal_type> m_on_create_signals;
    std::vector<signal_type> m_on_terminate_signals;

public:
    void is_dead_emit(Attacker*, Defender*, Applier&) const override;
    void is_dead_subscribe(signal_type) override;
    void is_dmg_emit(Attacker*, Defender*, Applier&) const override;
    void is_dmg_subscribe(signal_type) override;
    void is_hit_emit(Attacker*, Defender*, Applier&) const override;
    void is_hit_subscribe(signal_type) override;

    void on_create_emit(Attacker*, Defender*, Applier&) const override;
    void on_create_subscribe(signal_type) override;
    void on_terminate_emit(Attacker*, Defender*, Applier&) const override;
    void on_terminate_subscribe(signal_type) override;
};
