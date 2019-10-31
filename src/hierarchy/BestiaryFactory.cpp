#include "DescriptionOutput.hpp"
#include "FreeOutput.hpp"

#include "BestiaryFactory.hpp"
#include "Bestiary.hpp"

#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <cstddef>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen{rd()};

namespace
{
template <typename T>
void print_monster(const T& npc, const std::string& name)
{
    DescriptionOutput description_printer;
    FreeOutput stats_printer;

    std::cout << "Name: " << name << '\n';

    stats_printer.visit(npc);
    description_printer.visit(npc);

    std::cout << '\n';
}
}

BestiaryFactory::BestiaryFactory(std::size_t npc_num, IEventDispatcher& ed)
{
    using namespace std::string_literals;
    static std::uniform_int_distribution<> dis(0, 7);

    m_attackers.reserve(npc_num);
    m_defenders.reserve(npc_num);

    auto cmp = [] (const auto& lhv, const auto& rhv) {
      return lhv.ptr < rhv.ptr;
    };

    ed.is_dead_subscribe(
        [this, cmp] (Attacker* a_npc, Defender* d_npc, const Applier&)
        {
            assert(d_npc);
            auto binary_find_and_erase = [cmp](auto& vec, auto* key)
            {
                if (!key)
                    return;

                auto it = std::lower_bound(
                    vec.cbegin()
                  , vec.cend()
                  , BestiaryFactory::data<decltype(key)>{key, ""}
                  , cmp
                );

                if (it != vec.cend())
                    vec.erase(it);
            };

            binary_find_and_erase(m_attackers, a_npc);
            binary_find_and_erase(m_defenders, d_npc);
        }
    );

    for (std::size_t i = 0; i < npc_num; ++i)
    {
        switch (dis(gen))
        {
            case 0:
            {
                static std::uniform_int_distribution<> rat_hp_dis(10, 20);
                static std::uniform_int_distribution<> rat_strg_dis(1, 5);
                static std::size_t rat_num = 0;
                Rat* r = new Rat(rat_hp_dis(gen), rat_strg_dis(gen), ed);
                auto name = "Rat "s + std::to_string(rat_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 1:
            {
                static std::uniform_int_distribution<> hulk_hp_dis(15, 30);
                static std::uniform_int_distribution<> hulk_strg_dis(2, 5);
                static std::size_t hulk_num = 0;
                Hulk* r = new Hulk(hulk_hp_dis(gen), hulk_strg_dis(gen), ed);
                auto name = "Hulk "s + std::to_string(hulk_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 2:
            {
                static std::uniform_int_distribution<> mimic_hp_dis(7, 12);
                static std::size_t mimic_num = 0;
                Mimic* r = new Mimic(mimic_hp_dis(gen), ed);
                auto name = "Mimic "s + std::to_string(mimic_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 3:
            {
                static std::uniform_int_distribution<> slime_hp_dis(2, 5);
                static std::uniform_int_distribution<> slime_strg_dis(1, 3);
                static std::size_t slime_num = 0;
                Slime* r = new Slime(slime_hp_dis(gen), slime_strg_dis(gen), ed);
                auto name = "Slime "s + std::to_string(slime_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 4:
            {
                static std::uniform_int_distribution<> poison_cloud_strg_dis(1, 5);
                static std::size_t poison_cloud_num = 0;
                PoisonCloud* r = new PoisonCloud(poison_cloud_strg_dis(gen));
                auto name = "PoisonCloud "s + std::to_string(poison_cloud_num++);
                m_attackers.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 5:
            {
                static std::uniform_int_distribution<> door_hp_dis(5, 30);
                static std::size_t door_num = 0;
                Door* r = new Door(door_hp_dis(gen), ed);
                auto name = "Door "s + std::to_string(door_num++);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 6:
            {
                static std::uniform_int_distribution<> arm_rat_hp_dis(10, 20);
                static std::uniform_int_distribution<> arm_rat_strg_dis(1, 5);
                static std::uniform_int_distribution<> arm_rat_armor_dis(1, 3);
                static std::size_t arm_rat_num = 0;
                ArmoredRat* r = new ArmoredRat(arm_rat_hp_dis(gen), arm_rat_strg_dis(gen), arm_rat_armor_dis(gen), ed);
                auto name = "ArmoredRat "s + std::to_string(arm_rat_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
            case 7:
            {
                static std::uniform_int_distribution<> prat_hp_dis(10, 20);
                static std::uniform_int_distribution<> prat_strg_dis(1, 5);
                static std::uniform_int_distribution<> prat_rot_dis(1, 3);
                static std::size_t prat_num = 0;
                PlagueRat* r = new PlagueRat(prat_hp_dis(gen), prat_strg_dis(gen), prat_rot_dis(gen), ed);
                auto name = "PlagueRat "s + std::to_string(prat_num++);
                m_attackers.emplace_back(r, name);
                m_defenders.emplace_back(r, name);
                print_monster(*r, name);
                break;
            }
        }
    }

    std::sort(m_attackers.begin(), m_attackers.end(), cmp);
    std::sort(m_defenders.begin(), m_defenders.end(), cmp);
}

std::pair<std::string, const Attacker*> BestiaryFactory::get_rnd_attacker() const
{
    if (m_attackers.empty())
        throw std::runtime_error("List of attackers is empty.");

    std::uniform_int_distribution<> dis(0, m_attackers.size() - 1);

    std::size_t idx = dis(gen);
    auto it = m_attackers.cbegin() + idx;

    return {it->name, it->ptr};
}

std::pair<std::string, Defender*> BestiaryFactory::get_rnd_defender() const
{
    if (m_defenders.empty())
        throw std::runtime_error("List of defenders is empty.");

    std::uniform_int_distribution<> dis(0, m_defenders.size() - 1);

    std::size_t idx = dis(gen);
    auto it = m_defenders.cbegin() + idx;

    return {it->name, it->ptr};
}
