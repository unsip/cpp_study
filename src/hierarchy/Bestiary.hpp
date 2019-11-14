#pragma once

#include "IEventDispatcher.hpp"
#include "IBestiary.hpp"
#include "IApplier.hpp"

#include <cstddef>


class Rat : public Attacker, public Defender, public Applier
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
    void apply(const IBestiaryVisitor&) const override;
};


class ArmoredRat : public Rat
{
private:
    std::size_t m_armor;

public:
    ArmoredRat(std::size_t hp, std::size_t strength, std::size_t armor, const IEventDispatcher& ed)
        : Rat(hp, strength, ed), m_armor(armor)
    {}

    std::size_t get_armor() const { return m_armor; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) const override;
};


class PlagueRat : public Rat
{
private:
    std::size_t m_rot;
    std::size_t m_rot_stack = 0;

public:
    PlagueRat(std::size_t hp, std::size_t strength, std::size_t rot, const IEventDispatcher& ed)
        : Rat(hp, strength, ed), m_rot(rot)
    {}

    std::size_t attack() const override { return Rat::attack() + m_rot_stack; }
    std::size_t get_rot() const { return m_rot; }
    std::size_t get_rot_stack() const { return m_rot_stack; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) const override;
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
    void apply(const IBestiaryVisitor&) const override;
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
    void apply(const IBestiaryVisitor&) const override;
};


class ZombieMimic : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_reborn_hp;
    std::size_t m_reborn_cnt = 2;
    std::size_t m_strength;
    std::size_t m_res_cnt = 0;
    const IEventDispatcher& m_ed;

public:
    explicit ZombieMimic(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_reborn_hp(hp / 2), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) const override;
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
    void apply(const IBestiaryVisitor&) const override;
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
    void apply(const IBestiaryVisitor&) const override;
};


class SlimeQueen: public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    std::vector<SlimeShard*> m_brood;
    const IEventDispatcher& m_ed;

public:
    SlimeQueen(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_hp / 2; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) const override;
    void spawn_shards();
    std::vector<SlimeShard*> detach_shards();
};


class PoisonCloud : public Attacker, public Applier
{
private:
    std::size_t m_strength;

public:
    PoisonCloud(std::size_t strength) : m_strength(strength) {}

    std::size_t attack() const override { return m_strength; }
    void apply(const IBestiaryVisitor&) const override;
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
    void apply(const IBestiaryVisitor&) const override;
};

class StonePortal : public Door
{
public:
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) const override;
};
