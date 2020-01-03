#pragma once

#include "IBestiary.hpp"
#include "IEventDispatcher.hpp"

#include <tuple>
#include <memory>
#include <string>

class BestiaryFactory
{
public:
    explicit BestiaryFactory(IEventDispatcher& ed);
    std::tuple<std::string, std::shared_ptr<Attacker>, std::shared_ptr<Defender>> get_npc() const;

private:
    IEventDispatcher& m_ed;
};
