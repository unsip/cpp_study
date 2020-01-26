#pragma once

#include "IBestiary.hpp"
#include "IApplier.hpp"

#include <vector>
#include <cstddef>


class Rat : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;

public:
    Rat(std::size_t hp, std::size_t strength)
        : m_hp(hp), m_strength(strength)
    {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class ArmoredRat : public Rat
{
private:
    std::size_t m_armor;

public:
    ArmoredRat(std::size_t hp, std::size_t strength, std::size_t armor)
        : Rat(hp, strength), m_armor(armor)
    {}

    std::size_t get_armor() const { return m_armor; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class PlagueRat : public Rat
{
private:
    std::size_t m_rot;
    std::size_t m_rot_stack = 0;

public:
    PlagueRat(std::size_t hp, std::size_t strength, std::size_t rot)
        : Rat(hp, strength), m_rot(rot)
    {}

    std::size_t attack() const override { return Rat::attack() + m_rot_stack; }
    std::size_t get_rot() const { return m_rot; }
    std::size_t get_rot_stack() const { return m_rot_stack; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};


class Hulk : public Attacker, public Defender, public Applier
{
private:
    std::size_t m_hp;
    std::size_t m_strength;

public:
    Hulk(std::size_t hp, std::size_t strength)
        : m_hp(hp), m_strength(strength)
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

public:
    explicit Mimic(std::size_t hp)
        : m_hp(hp), m_strength(hp)
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

public:
    explicit ZombieMimic(std::size_t hp, std::size_t strength)
        : m_hp(hp), m_reborn_hp(hp / 2), m_strength(strength)
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

public:
    Slime(std::size_t hp, std::size_t strength)
        : m_hp(hp), m_strength(strength)
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

public:
    SlimeShard(std::size_t hp)
        : m_hp(hp)
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
    std::vector<SlimeShard*> m_brood;

public:
    SlimeQueen(std::size_t hp, std::size_t strength)
        : m_hp(hp), m_strength(strength)
    {}

    std::size_t attack() const override { return m_hp / 2; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void set_hp(std::size_t hp) { m_hp = hp; }
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
    void spawn_shards(std::size_t before_death_hp);
    std::vector<SlimeShard*> detach_shards();
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

public:
    explicit Door(std::size_t hp)
        : m_hp(hp)
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
    explicit StonePortal(std::size_t hp, std::size_t armor)
        : Door(hp), m_armor(armor)
    {}
    void hit(std::size_t dmg) override;
    void apply(const IBestiaryVisitor&) override;
};
