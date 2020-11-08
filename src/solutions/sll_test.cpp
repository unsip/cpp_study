#include "sll.hpp"

#include <random>
#include <string>
#include <iostream>

void sll_first_test(borealis::SingleLinkedList lst)
{
    borealis::SingleLinkedList empty_lst;

    for (const auto& val : lst)
        empty_lst.push_front(val);

    std::cout << lst << std::endl;
    std::cout << empty_lst << std::endl;

    borealis::SingleLinkedList inherited_lst{lst.begin(), lst.end()};
    std::cout << inherited_lst << std::endl;

    borealis::SingleLinkedList copied_lst{empty_lst};
    assert(copied_lst == empty_lst);
    assert(lst.get_size() == copied_lst.get_size());
    assert(lst.get_size() == 20);


    copied_lst.erase_after(copied_lst.begin());
    assert(copied_lst.get_size() == 19);
    assert(lst.get_size() == 20);
    assert(lst != copied_lst);

    lst = copied_lst;
    std::cout << lst << std::endl;
    std::cout << copied_lst << std::endl;
    std::cout << "HELLO" << std::endl;
    assert(lst == copied_lst);
    assert(lst.get_size() == 19);

    lst = lst;
    std::cout << lst << std::endl; //UB
}

borealis::SingleLinkedList create_lst(std::size_t sz = 20)
{
    static std::string names_choice [] = {"Andrey", "Kolya", "Vadim", "Costas", "Leysan", "Olli"};

    static std::random_device rd;
    static std::mt19937 gen{rd()};
    static std::uniform_int_distribution<> dis(0, sizeof names_choice / sizeof(std::string) - 1);

    borealis::SingleLinkedList new_lst;

    for (int i = 0; i < static_cast<int>(sz); ++i)
    {
        borealis::SingleLinkedList::Payload new_payload{i, names_choice[dis(gen)]};
        new_lst.push_front(new_payload);
    }

    return new_lst;
}

int main()
{
    sll_first_test(create_lst());
}
