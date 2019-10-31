#pragma once

#include "IApplier.hpp"
#include "IBestiary.hpp"

#include <functional>


class IEventDispatcher
{
public:
    using signal_type = std::function<void (Attacker*, Defender*, const Applier&)>;
    virtual void is_dead_emit(Attacker*, Defender*, const Applier&) const = 0;
    virtual void is_dead_subscribe(signal_type) = 0;
    virtual void is_dmg_emit(Attacker*, Defender*, const Applier&) const = 0;
    virtual void is_dmg_subscribe(signal_type) = 0;
    virtual void is_hit_emit(Attacker*, Defender*, const Applier&) const = 0;
    virtual void is_hit_subscribe(signal_type) = 0;
};
