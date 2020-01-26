#pragma once

#include <cstddef>
#include <functional>


template<class T>
class Id
{
private:
    const T* m_id;

public:
    explicit Id(const T* id) : m_id(id) {}
    bool operator== (const Id& rhv) const { return m_id == rhv.m_id; }
    bool operator< (const Id& rhv) const { return m_id < rhv.m_id; }
    std::size_t hash() const { return std::hash<const T*>()(m_id); }
};


class Comparable
{
protected:
    using IdType = Id<Comparable>;

private:
    virtual IdType id() const { return IdType{this}; }

protected:
    static IdType get_id(const Comparable& obj) { return obj.id(); }

public:
    friend bool is_same(const Comparable& lhv, const Comparable& rhv) { return lhv.id() == rhv.id(); }
    friend bool is_predecessor(const Comparable& lhv, const Comparable& rhv) { return lhv.id() < rhv.id(); }
    std::size_t hash() const { return id().hash(); }

    virtual ~Comparable() = 0;
};


inline Comparable::~Comparable() = default;


class Attacker : virtual public Comparable
{
public:
    virtual std::size_t attack() const = 0;
    virtual ~Attacker() = default;
};


class Defender : virtual public Comparable
{
public:
    virtual bool is_dead() const = 0;
    virtual std::size_t get_hp() const = 0;
    virtual void hit(std::size_t dmg) = 0;
    virtual ~Defender() = default;
};
