/*
 *  Basic pointer usage with manual memory allocation.
 */
#include <iostream>
#include <typeinfo>
#include <cstdlib>

int main()
{
    using namespace std;

    double* a = (double*)malloc(100 * sizeof(double));
    cout << "Pointed memory address: " << a << endl;
    cout << "Memory value (interpreted as " << typeid(a).name() << " or pointer to " << typeid(*a).name() <<  ") : " << *a << endl;
    cout << "Pointer address: " << &a << endl;

    double b;
    cout << "Another value address: " << &b << endl;

    string* strg = (string*)malloc(100 * sizeof(string));
    cout << "String pointer address: " << &strg << endl;
    cout << "String address: " << strg << endl;

    free(a);
    free(strg);
}
