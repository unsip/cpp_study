#pragma once

#include "IBestiary.hpp"
#include "IEventDispatcher.hpp"

#include <tuple>
#include <string>

class BestiaryFactory
{
public:
    explicit BestiaryFactory(IEventDispatcher& ed) noexcept : m_ed(ed)
    {}

    std::tuple<std::string, Attacker*, Defender*> get_npc() const;

private:
    IEventDispatcher& m_ed;
};
