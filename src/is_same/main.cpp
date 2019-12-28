#include "is_same.hpp"

#include <cassert>


int main()
{
    {
        A my_a;
        B my_b;
        C my_c;

        DecoratorB dec_b{&my_b};

        assert(my_a.do_a() == "A::do_a()");
        assert(my_b.do_b() == "B::do_b()");

        assert(my_c.do_a() == "C::do_a()");
        assert(my_c.do_b() == "C::do_b()");

        assert(dec_b.do_b() == "DecoratorB::B::do_b()");
    } // Case 1.

    {
        A my_a;
        B my_b;
        C my_c;

        IfaceA& iface_a(my_a);
        IfaceB& iface_b(my_b);
        IfaceA& a_iface_c(my_c);
        IfaceB& b_iface_c(my_c);

        DecoratorB dec_b{&iface_b};

        assert(iface_a.do_a() == "A::do_a()");
        assert(iface_b.do_b() == "B::do_b()");

        assert(a_iface_c.do_a() == "C::do_a()");
        assert(b_iface_c.do_b() == "C::do_b()");

        assert(dec_b.do_b() == "DecoratorB::B::do_b()");
    } // Case 2.


    {

        IfaceA* iface_a = new A();
        IfaceB* iface_b(new B());
        C* my_c = new C();
        IfaceA* a_iface_c(my_c);
        IfaceB* b_iface_c(my_c);

        IfaceB* dec_b = new DecoratorB(iface_b);

        assert(iface_a->do_a() == "A::do_a()");
        assert(iface_b->do_b() == "B::do_b()");

        assert(a_iface_c->do_a() == "C::do_a()");
        assert(b_iface_c->do_b() == "C::do_b()");

        assert(dec_b->do_b() == "DecoratorB::B::do_b()");

        assert(dynamic_cast<IfaceA*>(iface_b) == nullptr);

        assert(dynamic_cast<IfaceA*>(b_iface_c) == a_iface_c && b_iface_c->id() == a_iface_c->id());
        assert(dynamic_cast<IfaceB*>(a_iface_c) == b_iface_c && a_iface_c->id() == b_iface_c->id());

        assert(dynamic_cast<C*>(b_iface_c) == a_iface_c);
        assert(dynamic_cast<C*>(a_iface_c) == b_iface_c);

        assert(dec_b != iface_b && dec_b->id() == iface_b->id());

        assert(iface_a->id() != iface_b->id());
        assert(iface_a != a_iface_c && iface_a->id() != a_iface_c->id());
    } // Case 3.
}
