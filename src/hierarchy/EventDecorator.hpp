#pragma once

#include "IBestiary.hpp"
#include "IEventDispatcher.hpp"

#include <stdexcept>


class EventDecorator : public Defender//, public Applier
{
private:
    IEventDispatcher& m_ed;
    Attacker* m_att_npc;
    Applier& m_app_npc;
    Defender* m_def_npc;

public:
    EventDecorator(Attacker* att_npc, Defender* def_npc, Applier& app_npc, IEventDispatcher& ed)
        : m_ed(ed), m_att_npc(att_npc), m_app_npc(app_npc), m_def_npc(def_npc)
    {
        if (!def_npc)
            throw std::runtime_error("Invalid decorable object!");
    }
    ~EventDecorator()
    {
        delete m_def_npc;
    }

    EventDecorator(const EventDecorator&) = delete;
    EventDecorator& operator = (const EventDecorator&) = delete;


    bool is_dead() const override;
    std::size_t get_hp() const override;
    void hit(std::size_t dmg) override;
};
