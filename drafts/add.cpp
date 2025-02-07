#include <iostream>
#include <string>


//int add(int a, int b)
//{
//    return a + b;
//}
//
//std::string add(std::string a, std::string b)
//{
//    return a + b;
//}

template<typename T>
T add (T a, T b)
{
    return a + b;
}

class MyClass
{};


int main()
{
    std::string i = "Heh ";
    std::string j = " lol!";

    std::cout << "Result: " << add(i, j)<< std::endl;
    std::cout << "Result: " << add(4234, 43242)<< std::endl;
    add(MyClass(), MyClass());
    return 0;
}
