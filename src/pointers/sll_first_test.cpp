#include "sll_first_test.hpp"
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
