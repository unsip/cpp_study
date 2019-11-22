#pragma once

#include "IBestiary.hpp"

#include <string>

class IScene
{
public:
    virtual void add(const std::string& name, Attacker*, Defender*) = 0;
    virtual void remove(Attacker*, Defender*) = 0;
};

