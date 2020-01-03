#pragma once

#include "IBestiary.hpp"
#include "IEventDispatcher.hpp"

#include <memory>
#include <stdexcept>


class EventDecorator : public Defender//, public Applier
{
private:
    IEventDispatcher& m_ed;
    std::shared_ptr<Attacker> m_att_npc;
    std::shared_ptr<Applier> m_app_npc;
    std::shared_ptr<Defender> m_def_npc;

    IdType id() const override { return get_id(*m_def_npc); }

public:
    EventDecorator(std::shared_ptr<Attacker> att_npc, std::shared_ptr<Defender> def_npc, std::shared_ptr<Applier> app_npc, IEventDispatcher& ed)
        : m_ed(ed), m_att_npc(att_npc), m_app_npc(app_npc), m_def_npc(def_npc)
    {
        if (!def_npc)
            throw std::runtime_error("Invalid decorable object!");

        if (!app_npc)
            throw std::runtime_error("Missing Applier interface!");
    }

    EventDecorator(const EventDecorator&) = delete;
    EventDecorator& operator = (const EventDecorator&) = delete;


    bool is_dead() const override;
    std::size_t get_hp() const override;
    void hit(std::size_t dmg) override;
};
