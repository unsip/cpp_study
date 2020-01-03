#pragma once

#include <cstddef>


template<class T>
class Id
{
private:
    const T* m_id;

public:
    explicit Id(const T* id) : m_id(id) {}
    bool operator== (const Id& rhv) const { return m_id == rhv.m_id; }
    bool operator< (const Id& rhv) const { return m_id == rhv.m_id; }
};


class Compariable
{
protected:
    using IdType = Id<Compariable>;

private:
    virtual IdType id() const { return IdType{this}; }

protected:
    static IdType get_id(const Compariable& obj) { return obj.id(); }

public:
    friend bool is_same(const Compariable& lhv, const Compariable& rhv) { return lhv.id() == rhv.id(); }
    friend bool is_predecessor(const Compariable& lhv, const Compariable& rhv) { return lhv.id() < rhv.id(); }
};


class Attacker : virtual public Compariable
{
public:
    virtual std::size_t attack() const = 0;
    virtual ~Attacker() = default;
};


class Defender : virtual public Compariable
{
public:
    virtual bool is_dead() const = 0;
    virtual std::size_t get_hp() const = 0;
    virtual void hit(std::size_t dmg) = 0;
    virtual ~Defender() = default;
};
