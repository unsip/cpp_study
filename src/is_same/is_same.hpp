#pragma once

#include <string>


template <class Tag>
class Id
{
private:
    const void* m_id;
public:
    explicit Id(const void* id) : m_id(id) {}
    friend bool operator== (const Id& lhv, const Id& rhv) { return lhv.m_id == rhv.m_id; }
    friend bool operator!= (const Id& lhv, const Id& rhv) { return !(lhv == rhv); }
};


class MyTag {};


template <class Tag>
class Compariable
{
protected:
    using MyId = Id<Tag>;

    static MyId obj_id(const Compariable& obj) { return obj.id(); }

private:
    virtual MyId id() const { return MyId(this); }

public:
    virtual ~Compariable() = default;
    friend bool is(const Compariable& lhv, const Compariable& rhv) { return lhv.id() == rhv.id(); }
};


class IfaceA : virtual public Compariable<MyTag>
{
public:
    virtual std::string do_a() = 0;
    virtual ~IfaceA() = default;
};


class IfaceB : virtual public Compariable<MyTag>
{
public:
    virtual std::string do_b() = 0;
    virtual ~IfaceB() = default;
};


class A : public IfaceA
{
public:
    std::string do_a() override { return "A::do_a()"; }
};


class B : public IfaceB
{
public:
    std::string do_b() override { return "B::do_b()"; }
};


class C : public IfaceA, public IfaceB
{
public:
    std::string do_a() override { return "C::do_a()"; }

    std::string do_b() override { return "C::do_b()"; }
};


class DecoratorB : public IfaceB
{
private:
    IfaceB* m_iface_b;

    MyId id() const override { return Compariable::obj_id(*m_iface_b); }
public:
    DecoratorB(IfaceB* b) : m_iface_b(b) {}

    std::string do_b() override { return "DecoratorB::" + m_iface_b->do_b(); }
};
