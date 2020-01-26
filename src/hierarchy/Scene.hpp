#pragma once

#include "IScene.hpp"
#include "BestiaryFactory.hpp"


class Scene : public IScene
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    explicit Scene(std::size_t npc_num, const BestiaryFactory&, IEventDispatcher&);

    // Always think about assignment operators and copy/move constructors!
    Scene(const Scene&) = delete;
    Scene& operator = (const Scene&) = delete;

    ~Scene();

    /*
     * Precondition: Attacker and Defender are the same Comparable object.
     * Inserted object should not be on Scene.
     */
    void add(const std::string& name, std::shared_ptr<Attacker>, std::shared_ptr<Defender>) override;
    void remove(const Comparable&) override;

    std::pair<std::string, std::shared_ptr<Attacker>> get_rnd_attacker() const;
    std::pair<std::string, std::shared_ptr<Defender>> get_rnd_defender() const;
    bool is_last_man_standing() const;
};

