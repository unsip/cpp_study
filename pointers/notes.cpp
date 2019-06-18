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
ret_func (*f)(param_func, doulbe)
