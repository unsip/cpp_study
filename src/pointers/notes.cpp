/*
 *  Notes on type casting and higher-order functions semantics.
 */

int a = 10;
long b;

// static_cast<TO>(SOURCE_VAR)
a = static_cast<int>(b);

// Also may be used to cast between inherited classes instances
class Base {};
class Derived : public Base
{
    //...
};

Derived* d;
Base* b = static_cast<Base*>(d);

Base* b;
Derived* b = static_cast<Derived*>(d);

// Malloc and free declaration
void* malloc(size_t count);
void free(void*);

// Pointer type casting
void* tmp = malloc(42, sizeof(int)/8)
int* arr = (int*)tmp;

// reinterpret_cast
float c = 99.;
int d;

d = reinterpret_cast<int>(c);

// Structure declaration
struct my_struct {
    int id;
    float smth;
    std:string name;
};

// Higher-order functions (returning functions)
void (* f ) (int, double)
void (*f) (  int(*)()  , double)
int (* (*f) (  int(*)()  , double) )(int)

// Easier notation via typedef's
typedef int(*)(int) ret_func;
typedef int(*)() param_func;
ret_func (*f)(param_func, double)

// Address passthrough
Node& n;
n.next;
void bar(Node);

// Stricter than Node* coz address cant be null or uninitialized
void foo(Node&);

// Operator overloading
void foo(const Node&);
void foo(Node const&);

void foo(Node*);

// Overloading variants
void foo(const Node*);
void foo(Node const*);

void foo(Node* const);

void foo(Node const* const);
void foo(const Node* const);


// Why module-level operator overloading is better than doing so inside class
// declaration (for possible future casting between classes).
struct A
{
    /*explicit*/ A(B v);
    A operator+(A v);
};

A operator+(A lhv, A rhv);

struct B
{

};

A a1, a2;
B b1, b2;

a1 + a2
a2 + a1
a1 + b1
b1 + a1
a2 + b2
b1 + b2

