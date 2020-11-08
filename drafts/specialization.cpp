#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <cstring>

namespace {
    const char* names [] = { "Leysan", "Oleg", "Batya" };
}

template <typename Id, typename Name>
class Employee
{
public:
    Name get_name(Id id) { return names[id]; }
};

template <typename Name>
class Employee<std::string, Name>
{
public:
    Name get_name(Name id)
    {
        std::unordered_map<std::string, std::size_t> m
        {
            {"first", 0},
            {"second", 1},
            {"third", 2},
            {"0", 0},
            {"1", 1},
            {"2", 2},
        };

        return names[m.find(id)->second];
    }
};

template <typename T, typename E>
T my_pow(T x, E exp)
{
    T result = 1;

    for (T i = 1; i < exp; ++i)
    {
        result *= x;
    }

    return result;
}

template <>
class Employee<std::string, long long>
{
public:
    using ResultType = long long;
    ResultType get_name(std::string id)
    {
        std::unordered_map<std::string, std::size_t> m
        {
            {"first", 0},
            {"second", 1},
            {"third", 2},
            {"0", 0},
            {"1", 1},
            {"2", 2},
        };

        const char* s = names[m.find(id)->second];
        ResultType result = 0;
        auto len = strnlen(s, 10);

        if (!len)
            return result;

        char base = 'z' - 'a' + 1;
        std::cout << "Len: " << len << std::endl;
        std::cout << "Base: " << base << std::endl;

        std::unique_ptr<char[]> elem(new char [len]);

        for (std::size_t i = 0; i < len; ++i)
            elem[i] = tolower(s[i]);

        for (std::size_t i = 0; i < len; ++i)
        {
            ResultType digit = elem[len - i - 1] - 'a';
            std::cout << "Digit " << i << ": " << digit << std::endl;
            result += my_pow<ResultType>(base, i) * digit;
        }

        return result;
    }
};

int main()
{

    Employee<int, std::string> e;

    std::cout << e.get_name(0) << '\n' << e.get_name(1) << std::endl;


    Employee<std::string, std::string> s;
    std::cout << s.get_name("first") << '\n' << std::endl;
    std::cout << s.get_name("second") << '\n' << std::endl;
    std::cout << s.get_name("third") << '\n' << std::endl;

    Employee<std::string, long long> f;
    std::cout << f.get_name("first") << '\n' << std::endl;
    std::cout << f.get_name("second") << '\n' << std::endl;
    std::cout << f.get_name("third") << '\n' << std::endl;
}
