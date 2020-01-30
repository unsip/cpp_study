#pragma once

#include "IEventDispatcher.hpp"
#include "IBestiary.hpp"
#include "IApplier.hpp"

#include <cstddef>


class IRat : public Attacker, public Defender, public Applier
{
public:
    virtual std::string squeak() const = 0;
};


class Rat : public IRat
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    Rat(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
    std::string squeak() const override { return "Squeeeeak!"; }
};


class ArmoredRat : public IRat
{
private:
    Rat m_rat;
    std::size_t m_armor;

public:
    ArmoredRat(std::size_t hp, std::size_t strength, std::size_t armor, const IEventDispatcher& ed)
        : m_rat(hp, strength, ed), m_armor(armor)
    {}

    bool is_dead() const override { return m_rat.is_dead(); }
    std::size_t attack() const override { return m_rat.attack(); }
    std::size_t get_armor() const { return m_armor; }
    std::size_t get_hp() const override { return m_rat.get_hp(); }
    std::string squeak() const override { return m_rat.squeak(); }
    void apply(const IBestiaryVisitor&) override;
    void hit(std::size_t dmg) override;
};


class PlagueRat : public IRat
{
private:
    Rat m_rat;
    std::size_t m_rot;
    std::size_t m_rot_stack = 0;

public:
    PlagueRat(std::size_t hp, std::size_t strength, std::size_t rot, const IEventDispatcher& ed)
        : m_rat(hp, strength, ed), m_rot(rot)
    {}

    bool is_dead() const override { return m_rat.is_dead(); }
    std::size_t attack() const override { return m_rat.attack() + m_rot_stack; }
    std::size_t get_hp() const override { return m_rat.get_hp(); }
    std::size_t get_rot() const { return m_rot; }
    std::size_t get_rot_stack() const { return m_rot_stack; }
    std::string squeak() const override { return m_rat.squeak(); }
    void apply(const IBestiaryVisitor&) override;
    void hit(std::size_t dmg) override;
};


class Hulk : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    Hulk(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class Mimic : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    explicit Mimic(std::size_t hp, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(hp), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class ZombieMimic : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_reborn_hp;
    std::size_t m_reborn_cnt = 2;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    explicit ZombieMimic(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_reborn_hp(hp / 2), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
    bool reborn();
};


class Slime : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    Slime(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class SlimeShard : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    const IEventDispatcher& m_ed;

public:
    SlimeShard(std::size_t hp, const IEventDispatcher& ed)
        : m_hp(hp), m_ed(ed)
    {}

    std::size_t attack() const override { return m_hp / 2; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class SlimeQueen: public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    std::vector<std::unique_ptr<SlimeShard>> m_brood;
    const IEventDispatcher& m_ed;

public:
    SlimeQueen(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_hp / 2; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void set_hp(std::size_t hp) { m_hp = hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
    void spawn_shards(std::size_t before_death_hp);
    std::vector<std::unique_ptr<SlimeShard>> detach_shards();
};


class PoisonCloud : public Attacker, public Applier
{
private:
    std::size_t m_strength;

public:
    PoisonCloud(std::size_t strength) : m_strength(strength) {}

    std::size_t attack() const override { return m_strength; }
    void apply(const IBestiaryVisitor&) override;
};


class Door : public Defender, public Applier
{
private:
    std::size_t m_hp;
    const IEventDispatcher& m_ed;

public:
    explicit Door(std::size_t hp, const IEventDispatcher& ed)
        : m_hp(hp), m_ed(ed)
    {}

    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};

class StonePortal : public Door
{
private:
    std::size_t m_armor;

public:
    explicit StonePortal(std::size_t hp, std::size_t armor, const IEventDispatcher& ed)
        : Door(hp, ed), m_armor(armor)
    {}
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};
