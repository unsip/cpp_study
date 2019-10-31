#pragma once

#include "IEventDispatcher.hpp"
#include "IBestiary.hpp"

#include <cstddef>


class Rat : public Attacker, public Defender
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
};


class ArmoredRat : public Rat
{
private:
    std::size_t m_armor;

public:
    ArmoredRat(std::size_t hp, std::size_t strength, std::size_t armor, const IEventDispatcher& ed)
        : Rat(hp, strength, ed), m_armor(armor)
    {}

    void hit(std::size_t dmg) override;
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
    std::size_t get_rot_stack() { return m_rot_stack; }
    void hit(std::size_t dmg) override;
};


class Hulk : public Attacker, public Defender
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
};


class Mimic : public Attacker, public Defender
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
};


class Slime : public Attacker, public Defender
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
};


class SlimeShard : public Attacker, public Defender
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
};


class SlimeQueen: public Attacker, public Defender
{
private:
    std::size_t m_hp;
    std::size_t m_strength;
    const IEventDispatcher& m_ed;

public:
    SlimeQueen(std::size_t hp, std::size_t strength, const IEventDispatcher& ed)
        : m_hp(hp), m_strength(strength), m_ed(ed)
    {}

    std::size_t attack() const override { return m_hp / 2; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
};


class PoisonCloud : public Attacker
{
private:
    std::size_t m_strength;

public:
    PoisonCloud(std::size_t strength) : m_strength(strength) {}

    std::size_t attack() const override { return m_strength; }
};


class Door : public Defender
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
};

class StonePortal : public Door
{
public:
    void hit(std::size_t dmg) override;
};
