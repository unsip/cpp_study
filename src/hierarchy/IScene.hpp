#pragma once

#include "IBestiary.hpp"

#include <string>
#include <memory>

class IScene
{
public:
    virtual void add(const std::string& name, std::shared_ptr<Attacker>, std::shared_ptr<Defender>) = 0;
    virtual void remove(Attacker*, Defender*) = 0;
};

