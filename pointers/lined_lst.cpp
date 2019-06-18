/*
 *  Singly-linked list implementation.
 */
#include <iostream>

enum return_codes
{
    SUCCESS,
    ILLEGAL_ACTION,
    MALLOC_FAILURE,
    WRONG_PARAM
};

struct Node
{
    int id;
    std::string name;
    Node* next;
};

return_codes add_node_front(Node** head, int id)
{
    if (head == nullptr)
        return ILLEGAL_ACTION;

    Node* new_elem = (Node*)malloc(1 * sizeof(Node));

    if (new_elem == nullptr)
        return MALLOC_FAILURE;

    new_elem->id = id;
    new_elem->next = *head;
    *head = new_elem;

    return SUCCESS;
}

return_codes delete_node(Node** head, Node* elem)
{
    if (head == nullptr)
        return ILLEGAL_ACTION;

    if (*head == nullptr || elem == nullptr)
        return WRONG_PARAM;

    Node* current = *head;
    Node* previous = nullptr;
    while (current && current != elem)
    {
        previous = current;
        current = current->next;
    }

    if (current == nullptr)
        return WRONG_PARAM;

    if (previous)
        previous->next = current->next;
    else
        *head = current->next;

    free(current);

    return SUCCESS;
}

std::size_t lst_size(Node* head)
{
    std::size_t lst_size = 0;
    while (head)
    {
        ++lst_size;
        head = head->next;
    }

    return lst_size;
}

int main()
{
    Node* lst_head = nullptr;

    for (int i = 0; i < 100; ++i)
    {
        add_node_front(&lst_head, i);
    }
}
