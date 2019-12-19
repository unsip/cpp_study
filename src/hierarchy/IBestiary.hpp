#pragma once

#include <cstddef>

class Attacker
{
public:
    virtual std::size_t attack() const = 0;
    virtual ~Attacker() = default;
};


class Defender
{
public:
    virtual bool is_dead() const = 0;
    virtual std::size_t get_hp() const = 0;
    virtual void hit(std::size_t dmg) = 0;
    virtual ~Defender() = default;
};
