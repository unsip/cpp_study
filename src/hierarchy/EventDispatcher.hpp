#pragma once

#include "IEventDispatcher.hpp"

#include <vector>


class EventDispatcher : public IEventDispatcher
{
private:
    std::vector<signal_type> m_is_dead_signals;

public:
    void is_dead_emit(Attacker*, Defender*) const override;
    void is_dead_subscribe(signal_type) override;
};
