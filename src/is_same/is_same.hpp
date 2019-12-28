#pragma once

#include <string>


template <class Tag>
class Id
{
private:
    void* m_id;
public:
    explicit Id(void* id) : m_id(id) {}
    friend bool operator== (const Id& lhv, const Id& rhv) { return lhv.m_id == rhv.m_id; }
    friend bool operator!= (const Id& lhv, const Id& rhv) { return !(lhv == rhv); }
};


class MyTag {};


using MyId = Id<MyTag>;


class IfaceA
{
public:
    virtual std::string do_a() = 0;
    virtual MyId id() = 0 ;
    virtual ~IfaceA() = default;
};


class IfaceB
{
public:
    virtual std::string do_b() = 0;
    virtual MyId id() = 0 ;
    virtual ~IfaceB() = default;
};


class A : public IfaceA
{
public:
    MyId id() override { return MyId(this); }

    std::string do_a() override
    {
        return "A::do_a()";
    }
};


class B : public IfaceB
{
public:
    MyId id() override { return MyId(this); }

    std::string do_b() override
    {
        return "B::do_b()";
    }
};


class C : public IfaceA, public IfaceB
{
public:
    MyId id() override { return MyId(this); }

    std::string do_a() override
    {
        return "C::do_a()";
    }
    std::string do_b() override
    {
        return "C::do_b()";
    }
};


class DecoratorB : public IfaceB
{
private:
    IfaceB* m_iface_b;
public:
    DecoratorB(IfaceB* b) : m_iface_b(b) {}

    MyId id() override { return m_iface_b->id(); }

    std::string do_b() override
    {
        return "DecoratorB::" + m_iface_b->do_b();
    }
};
