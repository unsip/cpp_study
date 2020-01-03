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
        data(const std::shared_ptr<T>& p, std::string s) : ptr(p), name(std::move(s))
        {}
        std::shared_ptr<T> ptr;
        std::string name;
    };

    template <typename T>
    struct Cmp;

    template <typename T>
    struct PtrCmp;

    template <class Key>
    static auto binary_find(
        std::vector<data<Key>>& vec
      , const Key& key
    );

    template <class Key>
    static void binary_find_and_erase(
        std::vector<data<Key>>& vec
      , const Key& key
    );

    template <class Key>
    static void binary_find_and_insert(
        std::vector<data<Key>>& vec
      , const std::shared_ptr<Key>& key
      , std::string name
    );

    std::vector<data<Attacker>> m_attackers;
    std::vector<data<Defender>> m_defenders;

public:
    explicit Scene(std::size_t npc_num, const BestiaryFactory&, IEventDispatcher&);

    // Always think about assignment operators and copy/move constructors!
    Scene(const Scene&) = delete;
    Scene& operator = (const Scene&) = delete;

    void add(const std::string& name, std::shared_ptr<Attacker>, std::shared_ptr<Defender>) override;
    void remove(Attacker*, Defender*) override;

    std::pair<std::string, std::shared_ptr<Attacker>> get_rnd_attacker() const;
    std::pair<std::string, std::shared_ptr<Defender>> get_rnd_defender() const;
    bool is_last_man_standing() const;
};


