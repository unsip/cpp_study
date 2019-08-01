#include "sll_first_test.hpp"
#include "sll_second_test.hpp"

#include <random>
#include <string>

borealis::SingleLinkedList create_lst(std::size_t sz = 20)
{
    static std::string names_choice [] = {"Andrey", "Kolya", "Vadim", "Costas", "Leysan", "Olli"};

    static std::random_device rd;
    static std::mt19937 gen{rd()};
    static std::uniform_int_distribution<> dis(0, sizeof names_choice / sizeof(std::string) - 1);

    borealis::SingleLinkedList new_lst;

    for (int i = 0; i < sz; ++i)
    {
        borealis::SingleLinkedList::Payload new_payload{i, names_choice[dis(gen)]};
        new_lst.push_front(new_payload);
    }

    return new_lst;
}

int main()
{
    sll_first_test(create_lst());
    sll_second_test(create_lst(40));
}
