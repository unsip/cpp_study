#include "DescriptionOutput.hpp"
#include "FreeOutput.hpp"
#include "DamageVisitor.hpp"
#include "DeathVisitor.hpp"
#include "EventDecorator.hpp"

#include "BestiaryFactory.hpp"
#include "Bestiary.hpp"

#include <iostream>
#include <random>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen{rd()};

namespace
{
template <typename T>
void print_monster(T& npc, const std::string& name)
{
    DescriptionOutput description_printer;
    FreeOutput stats_printer;

    std::cout << "Name: " << name << '\n';

    stats_printer.visit(npc);
    description_printer.visit(npc);

    std::cout << '\n';
}

// Note: SlimeShards are being spawned only via SlimeQueen.
enum class Beast : char
{
    ArmoredRat,
    Door,
    Hulk,
    Mimic,
    PlagueRat,
    PoisonCloud,
    Rat,
    Slime,
    SlimeQueen,
    StonePortal,
    ZombieMimic
};
} // anonymous namespace

BestiaryFactory::BestiaryFactory(IEventDispatcher& ed) : m_ed(ed)
{
    ed.is_dmg_subscribe(
        [&ed] (Attacker*, Defender*, Applier& app_npc, std::size_t dmg)
        {
            DamageVisitor dmgv(ed, dmg);
            app_npc.apply(dmgv);
        }
    );
    ed.is_dead_subscribe(
        [&ed] (Attacker*, Defender*, Applier& app_npc)
        {
            DeathVisitor dv(ed);
            app_npc.apply(dv);
        }
    );
}

std::tuple<std::string, std::shared_ptr<Attacker>, std::shared_ptr<Defender>> BestiaryFactory::get_npc() const
{
    using namespace std::string_literals;
    static std::uniform_int_distribution<std::underlying_type<Beast>::type>
        dis(0, static_cast<std::underlying_type<Beast>::type>(Beast::ZombieMimic));

    std::tuple<std::string, std::shared_ptr<Attacker>, std::shared_ptr<Defender>> result{};
    std::shared_ptr<Applier> app_npc;

    switch (Beast{dis(gen)})
    {
        case Beast::Rat:
        {
            static std::uniform_int_distribution<> rat_hp_dis(10, 20);
            static std::uniform_int_distribution<> rat_strg_dis(1, 5);
            static std::size_t rat_num = 0;
            std::shared_ptr<Rat> r(new Rat(rat_hp_dis(gen), rat_strg_dis(gen), m_ed));
            app_npc = r;
            auto name = "Rat "s + std::to_string(rat_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::Hulk:
        {
            static std::uniform_int_distribution<> hulk_hp_dis(15, 30);
            static std::uniform_int_distribution<> hulk_strg_dis(2, 5);
            static std::size_t hulk_num = 0;
            std::shared_ptr<Hulk> r(new Hulk(hulk_hp_dis(gen), hulk_strg_dis(gen), m_ed));
            app_npc = r;
            auto name = "Hulk "s + std::to_string(hulk_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::Mimic:
        {
            static std::uniform_int_distribution<> mimic_hp_dis(7, 12);
            static std::size_t mimic_num = 0;
            std::shared_ptr<Mimic> r(new Mimic(mimic_hp_dis(gen), m_ed));
            app_npc = r;
            auto name = "Mimic "s + std::to_string(mimic_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::Slime:
        {
            static std::uniform_int_distribution<> slime_hp_dis(2, 5);
            static std::uniform_int_distribution<> slime_strg_dis(1, 3);
            static std::size_t slime_num = 0;
            std::shared_ptr<Slime> r(new Slime(slime_hp_dis(gen), slime_strg_dis(gen), m_ed));
            app_npc = r;
            auto name = "Slime "s + std::to_string(slime_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::PoisonCloud:
        {
            static std::uniform_int_distribution<> poison_cloud_strg_dis(1, 5);
            static std::size_t poison_cloud_num = 0;
            std::shared_ptr<PoisonCloud> r(new PoisonCloud(poison_cloud_strg_dis(gen)));
            app_npc = r;
            auto name = "PoisonCloud "s + std::to_string(poison_cloud_num++);
            result = decltype(result){name, r, nullptr};
            print_monster(*r, name);
            break;
        }
        case Beast::Door:
        {
            static std::uniform_int_distribution<> door_hp_dis(5, 30);
            static std::size_t door_num = 0;
            std::shared_ptr<Door> r(new Door(door_hp_dis(gen), m_ed));
            app_npc = r;
            auto name = "Door "s + std::to_string(door_num++);
            result = decltype(result){name, nullptr, r};
            print_monster(*r, name);
            break;
        }
        case Beast::ArmoredRat:
        {
            static std::uniform_int_distribution<> arm_rat_hp_dis(10, 20);
            static std::uniform_int_distribution<> arm_rat_strg_dis(1, 5);
            static std::uniform_int_distribution<> arm_rat_armor_dis(1, 3);
            static std::size_t arm_rat_num = 0;
            std::shared_ptr<ArmoredRat> r(new ArmoredRat(arm_rat_hp_dis(gen), arm_rat_strg_dis(gen), arm_rat_armor_dis(gen), m_ed));
            app_npc = r;
            auto name = "ArmoredRat "s + std::to_string(arm_rat_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::PlagueRat:
        {
            static std::uniform_int_distribution<> prat_hp_dis(10, 20);
            static std::uniform_int_distribution<> prat_strg_dis(1, 5);
            static std::uniform_int_distribution<> prat_rot_dis(1, 3);
            static std::size_t prat_num = 0;
            std::shared_ptr<PlagueRat> r(new PlagueRat(prat_hp_dis(gen), prat_strg_dis(gen), prat_rot_dis(gen), m_ed));
            app_npc = r;
            auto name = "PlagueRat "s + std::to_string(prat_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::StonePortal:
        {
            static std::uniform_int_distribution<> sportal_hp_dis(40, 80);
            static std::uniform_int_distribution<> sportal_armor_dis(2, 5);
            static std::size_t sportal_num = 0;
            std::shared_ptr<StonePortal> r(new StonePortal(sportal_hp_dis(gen), sportal_armor_dis(gen), m_ed));
            app_npc = r;
            auto name = "StonePortal "s + std::to_string(sportal_num++);
            result = decltype(result){name, nullptr, r};
            print_monster(*r, name);
            break;
        }
        case Beast::SlimeQueen:
        {
            static std::uniform_int_distribution<> squeen_hp_dis(30, 60);
            static std::uniform_int_distribution<> squeen_strg_dis(5, 10);
            static std::size_t squeen_num = 0;
            std::shared_ptr<SlimeQueen> r(new SlimeQueen(squeen_hp_dis(gen), squeen_strg_dis(gen), m_ed));
            app_npc = r;
            auto name = "SlimeQueen "s + std::to_string(squeen_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
        case Beast::ZombieMimic:
        {
            static std::uniform_int_distribution<> zmimic_hp_dis(10, 20);
            static std::uniform_int_distribution<> zmimic_strg_dis(1, 5);
            static std::size_t zmimic_num = 0;
            std::shared_ptr<ZombieMimic> r(new ZombieMimic(zmimic_hp_dis(gen), zmimic_strg_dis(gen), m_ed));
            app_npc = r;
            auto name = "ZombieMimic "s + std::to_string(zmimic_num++);
            result = decltype(result){name, r, r};
            print_monster(*r, name);
            break;
        }
    }

    assert(app_npc && "Applier reference can't be nullptr!");
    std::shared_ptr<Defender>& def_npc = std::get<2>(result);
    if (def_npc)
        def_npc = std::shared_ptr<Defender>(new EventDecorator(std::get<1>(result), def_npc, app_npc, m_ed));

    return result;
}

