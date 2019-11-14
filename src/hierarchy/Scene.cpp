#include "Scene.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>
#include <cstddef>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen{rd()};

template <typename Key>
struct Scene::Cmp
{
    bool operator()(
        const Scene::data<Key*>& lhv
      , const Scene::data<Key*>& rhv
    ) const
    {
        return lhv.ptr < rhv.ptr;
    }
};

template <class Key>
void Scene::binary_find_and_erase(
    std::vector<data<Key*>>& vec
  , Key* key
)
{
    if (!key)
        return;

    Cmp<Key> cmp;
    auto it = std::lower_bound(
        vec.cbegin()
      , vec.cend()
      , Scene::data<Key*>{key, ""}
      , cmp
    );

    if (it != vec.cend())
        vec.erase(it);
}

Scene::Scene(std::size_t npc_num, const BestiaryFactory& fact, IEventDispatcher& ed)
{
    m_attackers.reserve(npc_num);
    m_defenders.reserve(npc_num);

    for (std::size_t i = 0; i < npc_num; ++i)
    {
        auto [name, aiface, diface] = fact.get_npc();
        /// @todo all event subscriptions
        add(std::move(name), aiface, diface);
    }

    std::sort(m_attackers.begin(), m_attackers.end(), Cmp<Attacker>{});
    std::sort(m_defenders.begin(), m_defenders.end(), Cmp<Defender>{});
}

void Scene::add(std::string name, Attacker* aiface, Defender* diface)
{
    if (aiface)
        m_attackers.emplace_back(aiface, std::move(name));
    if (diface)
        m_defenders.emplace_back(diface, std::move(name));
}

void Scene::remove(Attacker* aiface, Defender* diface)
{
    /// @todo What about nullptrs?
    binary_find_and_erase(m_attackers, aiface);
    binary_find_and_erase(m_defenders, diface);
}

bool Scene::is_last_man_standing() const
{
    return m_defenders.empty()
        || m_attackers.empty()
        || (m_attackers.size() == 1
            && m_defenders.size() == 1
            && m_defenders.front().name == m_attackers.front().name
        );
}

std::pair<std::string, const Attacker*> Scene::get_rnd_attacker() const
{
    if (m_attackers.empty())
        throw std::runtime_error("List of attackers is empty.");

    std::uniform_int_distribution<> dis(0, m_attackers.size() - 1);

    std::size_t idx = dis(gen);
    auto it = m_attackers.cbegin() + idx;

    return {it->name, it->ptr};
}

std::pair<std::string, Defender*> Scene::get_rnd_defender() const
{
    if (m_defenders.empty())
        throw std::runtime_error("List of defenders is empty.");

    std::uniform_int_distribution<> dis(0, m_defenders.size() - 1);

    std::size_t idx = dis(gen);
    auto it = m_defenders.cbegin() + idx;

    return {it->name, it->ptr};
}
