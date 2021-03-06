#pragma once

#include "IApplier.hpp"
#include "IBestiary.hpp"

#include <functional>
#include <string>


class IEventDispatcher
{
public:
    using signal_type = std::function<void (Attacker*, Defender*, Applier&)>;
    using dmg_signal = std::function<void (Attacker*, Defender*, Applier&, std::size_t dmg)>;
    using create_signal = std::function<void (Attacker*, Defender*, Applier&, std::string name)>;
    virtual void is_dead_emit(Attacker*, Defender*, Applier&) const = 0;
    virtual void is_dead_subscribe(signal_type) = 0;
    virtual void is_dmg_emit(Attacker*, Defender*, Applier&, std::size_t) const = 0;
    virtual void is_dmg_subscribe(dmg_signal) = 0;
    virtual void is_hit_emit(Attacker*, Defender*, Applier&) const = 0;
    virtual void is_hit_subscribe(signal_type) = 0;

    virtual void on_create_emit(Attacker*, Defender*, Applier&, std::string name) const = 0;
    virtual void on_create_subscribe(create_signal) = 0;
    virtual void on_terminate_emit(Attacker*, Defender*, Applier&) const = 0;
    virtual void on_terminate_subscribe(signal_type) = 0;
};
