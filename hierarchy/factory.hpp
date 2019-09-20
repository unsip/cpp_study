#pragma once

#include "ibestiary.hpp"

#include <string>
#include <vector>
#include <utility>

class BestiaryFactory
{
private:
    std::vector<std::pair<std::string, Attacker*>> m_attackers;
    std::vector<std::pair<std::string, Defender*>> m_defenders;
public:
    explicit BestiaryFactory(std::size_t n);
    std::pair<std::string, const Attacker*> get_rnd_attacker() const;
    std::pair<std::string, Defender*> get_rnd_defender() const;
};
