#pragma once

#include "IBestiary.hpp"

#include <functional>


class IEventDispatcher
{
public:
    using signal_type = std::function<void (Attacker*, Defender*)>;
    virtual void is_dead_emit(Attacker*, Defender*) const = 0;
    virtual void is_dead_subscribe(signal_type) = 0;
};

//
//
// m_attackers.remove(OldQueen)
// m_defenders.remove(OldQueen)
//
//
