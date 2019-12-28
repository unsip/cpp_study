#pragma once

#include <string>

unsigned get_new_id()
{
    static unsigned id = 0;
    return id++;
}


class IfaceA
{
public:
    virtual std::string do_a() = 0;
    virtual unsigned id() = 0 ;
    virtual ~IfaceA() = default;
};


class IfaceB
{
public:
    virtual std::string do_b() = 0;
    virtual unsigned id() = 0 ;
    virtual ~IfaceB() = default;
};


class A : public IfaceA
{
private:
    unsigned m_id;

public:
    A()
        : m_id(get_new_id())
    {}

    A(const A&)
        : m_id(get_new_id())
    {}

    A& operator= (const A&)
    { return *this; }

    unsigned id() override { return m_id; }

    std::string do_a() override
    {
        return "A::do_a()";
    }
};


class B : public IfaceB
{
private:
    unsigned m_id;

public:
    B() : m_id(get_new_id()) {}
    B(const B&) : m_id(get_new_id()) {}

    B& operator= (const B&) { return *this; }

    unsigned id() override { return m_id; }

    std::string do_b() override
    {
        return "B::do_b()";
    }
};


class C : public IfaceA, public IfaceB
{
private:
    unsigned m_id;

public:
    C() : m_id(get_new_id()) {}
    C(const C&) : m_id(get_new_id()) {}

    C& operator= (const C&) { return *this; }

    unsigned id() override { return m_id; }

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

    unsigned id() override { return m_iface_b->id(); }

    std::string do_b() override
    {
        return "DecoratorB::" + m_iface_b->do_b();
    }
};
