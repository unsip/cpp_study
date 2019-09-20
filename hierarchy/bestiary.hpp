#pragma once

#include "ibestiary.hpp"

#include <cstddef>


class Rat : public Attacker, public Defender
{
private:
    std::size_t m_hp;
    std::size_t m_strength;

public:
    Rat(std::size_t hp, std::size_t strength) : m_hp(hp), m_strength(strength) {}

    std::size_t attack() const override { return m_strength; }
    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
};


class Hulk : public Attacker, public Defender
{
private:
    std::size_t m_hp;
    std::size_t m_strength;

public:
    Hulk(std::size_t hp, std::size_t strength) : m_hp(hp), m_strength(strength) {}

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

public:
    explicit Mimic(std::size_t hp) : m_hp(hp), m_strength(hp) {}

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

public:
    Slime(std::size_t hp, std::size_t strength) : m_hp(hp), m_strength(strength) {}

    std::size_t attack() const override { return m_strength; }
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

public:
    explicit Door(std::size_t hp) : m_hp(hp) {}

    bool is_dead() const override { return m_hp == 0; }
    std::size_t get_hp() const override { return m_hp; }
    void hit(std::size_t dmg) override;
};
