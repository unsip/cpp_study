#include "factory.hpp"
#include "bestiary.hpp"

#include <random>
#include <cstddef>

static std::random_device rd;
static std::mt19937 gen{rd()};

BestiaryFactory::BestiaryFactory(std::size_t n)
{
    using namespace std::string_literals;
    static std::uniform_int_distribution<> dis(0, 5);

    m_attackers.reserve(n);
    m_defenders.reserve(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        switch (dis(gen))
        {
            case 0:
            {
                static std::uniform_int_distribution<> rat_hp_dis(10, 20);
                static std::uniform_int_distribution<> rat_strg_dis(1, 5);
                static std::size_t rat_num = 0;
                Rat* r = new Rat(rat_hp_dis(gen), rat_strg_dis(gen));
                auto name = "Rat "s + std::to_string(rat_num++);
                m_attackers.push_back({name, r});
                m_defenders.push_back({name, r});
            }
            case 1:
            {
                static std::uniform_int_distribution<> hulk_hp_dis(15, 30);
                static std::uniform_int_distribution<> hulk_strg_dis(2, 5);
                static std::size_t hulk_num = 0;
                Hulk* r = new Hulk(hulk_hp_dis(gen), hulk_strg_dis(gen));
                auto name = "Hulk "s + std::to_string(hulk_num++);
                m_attackers.push_back({name, r});
                m_defenders.push_back({name, r});
            }
            case 2:
            {
                static std::uniform_int_distribution<> mimic_hp_dis(7, 12);
                static std::size_t mimic_num = 0;
                Mimic* r = new Mimic(mimic_hp_dis(gen));
                auto name = "Mimic "s + std::to_string(mimic_num++);
                m_attackers.push_back({name, r});
                m_defenders.push_back({name, r});
            }
            case 3:
            {
                static std::uniform_int_distribution<> slime_hp_dis(2, 5);
                static std::uniform_int_distribution<> slime_strg_dis(1, 3);
                static std::size_t slime_num = 0;
                Slime* r = new Slime(slime_hp_dis(gen), slime_strg_dis(gen));
                auto name = "Slime "s + std::to_string(slime_num++);
                m_attackers.push_back({name, r});
                m_defenders.push_back({name, r});
            }
            case 4:
            {
                static std::uniform_int_distribution<> poison_cloud_strg_dis(1, 5);
                static std::size_t poison_cloud_num = 0;
                PoisonCloud* r = new PoisonCloud(poison_cloud_strg_dis(gen));
                auto name = "PoisonCloud "s + std::to_string(poison_cloud_num++);
                m_attackers.push_back({name, r});
            }
            case 5:
            {
                static std::uniform_int_distribution<> door_hp_dis(5, 30);
                static std::size_t door_num = 0;
                Door* r = new Door(door_hp_dis(gen));
                auto name = "Door "s + std::to_string(door_num++);
                m_defenders.push_back({name, r});
            }
        }
    }
}

/*
 * @todo:  
 * 1 если вектор пуст кидаем эксепшен
 * 2 до тех пор пока рандомный элемент из массива is_dead удаляем его из
 * массива и повторяем все с шага 1
 *
 * 3 Реализовать таким образом get_rnd_attacker и get_rnd_defender
 * 4 В функции main на каждом шаге печатать лог того что произошло
 *    "Имя Аттакера" атаковал "Имя Дефендера". Ну и изменение статуса дефендера в произвольном формате
 * 5 Скомпилировать, слинковать, запустить
 * 6 Прикинуть как освободить за собой память
 */
 */
std::pair<std::string, const Attacker*> BestiaryFactory::get_rnd_attacker() const
{
    std::uniform_int_distribution<> dis(0, m_attackers.size());

    std::size_t idx = dis(gen);
    auto it = m_attackers.cbegin() + idx;

    if (it->second->is_dead())
    {
        m_attackers.erase(it);
    }

    return a;
}

std::pair<std::string, Defender*> BestiaryFactory::get_rnd_defender() const
{
    std::uniform_int_distribution<> dis(0, m_defenders.size());

    return m_defenders[dis(gen)];
}
