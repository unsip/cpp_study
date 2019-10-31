#pragma once

#include "IBestiary.hpp"
#include "IEventDispatcher.hpp"

#include <string>
#include <vector>
#include <utility>

class BestiaryFactory
{
private:
    template <typename T>
    struct data
    {
        data(const T& p, std::string s) : ptr(p), name(std::move(s))
        {}
        T ptr;
        std::string name;
    };
    std::vector<data<Attacker*>> m_attackers;
    std::vector<data<Defender*>> m_defenders;
public:
    BestiaryFactory(std::size_t npc_num, IEventDispatcher& ed);
    std::pair<std::string, const Attacker*> get_rnd_attacker() const;
    std::pair<std::string, Defender*> get_rnd_defender() const;
};
