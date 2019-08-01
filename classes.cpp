struct A : /*private*/ B {
// public:
};

class B : /*public*/ A {
// private:
};



public: - public iface
private: - implementation details
protected:

void Foo::bar() const {
    m_r = bar_impl(m_i, m_a, m_n)
}


// Virtual functions
class Base
{
public:
    int a = 0;
    virtual void foo() const
    {
        cout << "Base::foo\n";
    }
    void baz() const
    {
        cout << "Base::baz\n"
    }

    virtual void pur() = 0;

    virtual ~Base() = delete; // I gonna use dynamic polymorphism
};

class Derived : public Base
{
public:
    int a = 1;
    /*virtual*/ void foo() const
    {
        cout << "Derived::foo\n";
    }
    void baz() const
    {
        cout << "Derived::baz\n";
    }
};

// main
Base* b = new Derived();
b->foo(); // Derived::foo
b->baz(); // Base::baz
delete b; // if no virtual dtor - FUCKUP. Memory leak!!!!!!!!!

Derived d1;
Base& b1 = d1;
b1.foo(); // Derived::foo
b1.buz(); // Base::baz
b1.Base::foo(); // Base::foo

d1.buz(); // Derived::baz
d1.Base::buz(); // Base::baz


// Multiple inheritance
class Enemy
{
public:
    // returns winner
    virtual Person& fight(const Person&) = 0;
    virtual ~Enemy() {};
};

class Npc
{
public:
    virtual void talk(const Person&) = 0;
    virtual ~Npc() {}
};

class AlienCitizen : public Npc, public Enemy
{
public:
    void talk(const Person&) override;
    Person& fight(const Person&) override;
};

class FriendlyRobotNinjaBubble : public Npc, public Enemy
{};

class PoorGirl : public Npc
{};

class DialogDispatcher
{
public:
    void add_npc(Npc&);
};

class BattleArena
{
public:
    void add_fighter(Enemy&);

};

// main
AlienCitezen ac;
PoorGirl girl;
BattleArena ba;
ba.add_fighter(ac);
DialogDispatcher.add_npc(ac);
DialogDispatcher.add_npc(girl);

