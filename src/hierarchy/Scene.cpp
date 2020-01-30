#include "Scene.hpp"
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include <cstddef>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen{rd()};


template <typename T>
struct data
{
private:
    std::shared_ptr<T> m_sp;
    const Comparable& m_r;
public:
    data(std::shared_ptr<T> p) : m_sp(std::move(p)), m_r(*m_sp) {}
    data(const Comparable& r) : m_r(r) {}

    operator std::shared_ptr<T> () const { return m_sp; }
    operator const Comparable& () const { return m_r; }
    friend bool operator==(const data& lhv, const data& rhv) {
        return is_same(lhv.m_r, rhv.m_r);
    }
    friend class std::hash<data<T>>;
};


namespace std
{
    template <>
    struct hash<::data<Attacker>>
    {
        size_t operator() (const ::data<Attacker>& d) const noexcept
        {
            return d.m_r.hash();
        }
    };

    template <>
    struct hash<::data<Defender>>
    {
        size_t operator() (const ::data<Defender>& d) const noexcept
        {
            return d.m_r.hash();
        }
    };
}


struct Scene::Impl
{
    struct Cmp
    {
        bool operator()(
            const std::shared_ptr<Comparable>& lhv,
            const std::shared_ptr<Comparable>& rhv
        ) const
        {
            if (lhv && rhv)
                return is_same(*lhv, *rhv);

            return false;
        }
    };

    struct Hash
    {
        size_t operator() (const std::shared_ptr<Comparable>& ptr) const noexcept
        {
            return ptr->hash();
        }
    };

    std::unordered_map<std::shared_ptr<Comparable>, std::string, Hash, Cmp> m_names;
    std::unordered_set<data<Attacker>> m_attackers;
    std::unordered_set<data<Defender>> m_defenders;
};


Scene::Scene(std::size_t npc_num, const BestiaryFactory& fact, IEventDispatcher& ed)
    : m_pimpl(new Scene::Impl())
{
    m_pimpl->m_attackers.reserve(npc_num);
    m_pimpl->m_defenders.reserve(npc_num);

    ed.is_dead_subscribe(
        [this] (Attacker* a_npc, Defender* d_npc, Applier&)
        {
            assert(a_npc || d_npc);
            remove(a_npc? static_cast<Comparable&>(*a_npc) : static_cast<Comparable&>(*d_npc));
        }
    );

    ed.on_create_subscribe(
        [this] (std::shared_ptr<Attacker> a_npc, std::shared_ptr<Defender> d_npc, Applier&)
        {
            ///@todo Not thread safe
            static std::size_t cnt= 0;
            std::stringstream ss;
            ss << "Newborn " << cnt++;
            add(ss.str(), a_npc, d_npc);
        }
    );

    for (std::size_t i = 0; i < npc_num; ++i)
    {
        auto [name, aiface, diface] = fact.get_npc();
        /// @todo all event subscriptions
        add(std::move(name), aiface, diface);
    }
}

Scene::~Scene() = default;

// Implying that names are unique!
void Scene::add(const std::string& name, std::shared_ptr<Attacker> aiface, std::shared_ptr<Defender> diface)
{
    // TODO: Think whether these constraints are interface contracts (in which
    // case we should throw exception).
    assert(aiface || diface);
    assert(!(aiface && diface) || is_same(*aiface, *diface));

    std::shared_ptr<Comparable> ichoice;
    if (aiface)
        ichoice = aiface;
    else
        ichoice = diface;

    auto pr = m_pimpl->m_names.insert(std::pair<std::shared_ptr<Comparable>, std::string>{ichoice, name});
    assert(pr.second && "Unique constraint violated! We can't have multiple creatures in the same map.");

    if (aiface)
    {
        auto pr = m_pimpl->m_attackers.insert(aiface).second;
        assert(pr && "Unique Attacker constraint violated!");
    }

    if (diface)
    {
        auto pr = m_pimpl->m_defenders.insert(diface).second;
        assert(pr && "Unique Defender constraint violated!");
    }
}

void Scene::remove(const Comparable& obj)
{
    m_pimpl->m_attackers.erase(obj);
    m_pimpl->m_defenders.erase(obj);
}

bool Scene::is_last_man_standing() const
{
    return m_pimpl->m_defenders.empty()
        || m_pimpl->m_attackers.empty() || (m_pimpl->m_attackers.size() == 1
            && m_pimpl->m_defenders.size() == 1
            && is_same(*m_pimpl->m_defenders.cbegin(), *m_pimpl->m_attackers.cbegin())
        );
}

std::pair<std::string, std::shared_ptr<Attacker>> Scene::get_rnd_attacker() const
{
    if (m_pimpl->m_attackers.empty())
        throw std::runtime_error("List of attackers is empty.");

    std::uniform_int_distribution<> dis(0, m_pimpl->m_attackers.size() - 1);

    std::size_t idx = dis(gen);
    // TODO: Get rid of O(n) iteration complexity. Hashmap does not support
    // random access iterator.
    auto it = std::next(m_pimpl->m_attackers.cbegin(), idx);
    auto name_it = m_pimpl->m_names.find(std::shared_ptr<Attacker>(*it));

    assert(name_it != m_pimpl->m_names.cend());

    return {name_it->second, *it};
}

std::pair<std::string, std::shared_ptr<Defender>> Scene::get_rnd_defender() const
{
    if (m_pimpl->m_defenders.empty())
        throw std::runtime_error("List of defenders is empty.");

    std::uniform_int_distribution<> dis(0, m_pimpl->m_defenders.size() - 1);

    std::size_t idx = dis(gen);

    auto it = std::next(m_pimpl->m_defenders.cbegin(), idx);
    auto name_it = m_pimpl->m_names.find(std::shared_ptr<Defender>(*it));

    assert(name_it != m_pimpl->m_names.cend());

    return {name_it->second, *it};
}
