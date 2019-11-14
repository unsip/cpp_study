#pragma once

#include "IScene.hpp"
#include "BestiaryFactory.hpp"

#include <vector>


class Scene : public IScene
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

    template <typename T>
    struct Cmp;

    template <class Key>
    static void binary_find_and_erase(
        std::vector<data<Key*>>& vec
      , Key* key
    );

    std::vector<data<Attacker*>> m_attackers;
    std::vector<data<Defender*>> m_defenders;

public:
    explicit Scene(std::size_t npc_num, const BestiaryFactory&, IEventDispatcher&);
    void add(std::string name, Attacker*, Defender*) override;
    void remove(Attacker*, Defender*) override;

    std::pair<std::string, const Attacker*> get_rnd_attacker() const;
    std::pair<std::string, Defender*> get_rnd_defender() const;
    bool is_last_man_standing() const;
};


