/*
 *  Singly-linked list implementation.
 */
#include "single_linked_lst.hpp"
#include <cassert>
#include <iostream>

namespace borealis
{

SingleLinkedList::Iter SingleLinkedList::begin()
{
    return Iter{m_head};
}

SingleLinkedList::Iter SingleLinkedList::end()
{
    return Iter{nullptr};
}

SingleLinkedList::ConstIter SingleLinkedList::begin() const
{
    return ConstIter{Iter{m_head}};
}

SingleLinkedList::ConstIter SingleLinkedList::end() const
{
    return ConstIter{Iter{nullptr}};
}

SingleLinkedList::Node* SingleLinkedList::copy(const SingleLinkedList::Node* current)
{
    Node* head = nullptr;
    Node* previous = nullptr;
    try
    {
        while (current)
        {
            // TODO: What if exception occures?
            Node* new_elem = new Node(current->payload);
            if (!head)
                head = new_elem;
            else
            {
                assert(head && "Current branch is visited on second and later iterations only!");
                assert(previous && "Current branch is visited on second and later iterations only!");
                previous->next = new_elem;
            }
            previous = new_elem;
            current = current->next;
        }
    }
    catch (...)
    {
        clear(head);
        throw;
    }

    return head;
}

SingleLinkedList::SingleLinkedList(const SingleLinkedList::Payload& initial, std::size_t count)
{
    Node* current = m_head;

    try
    {
        for (std::size_t i = 0; i < count; ++i)
        {
            Node* new_elem = new Node(initial);
            if (current == nullptr)
            {
                current = new_elem;
                m_head = current;
            }
            else
            {
                current->next = new_elem;
                current = current->next;
            }
        }
    }
    catch (...)
    {
        clear();
        throw;
    }

    m_lst_size = count;
}

// Payload dtor noexcept, UB (undefined behaviour) otherwise
void SingleLinkedList::clear() noexcept
{
    clear(m_head);
    m_lst_size = 0;
}

// static
void SingleLinkedList::clear(const Node* head) noexcept
{
    const Node* current = head;

    while (current)
    {
        const Node* tmp = current;
        current = current->next;
        delete tmp;
    }
}

// Copy ctor
SingleLinkedList::SingleLinkedList(const SingleLinkedList& lst)
    : m_lst_size{lst.m_lst_size}, m_head{copy(lst.m_head)}
{}

SingleLinkedList& SingleLinkedList::operator = (SingleLinkedList rhv)
{
    rhv.swap(*this);
    return *this;
}

void SingleLinkedList::swap(SingleLinkedList& rhv) noexcept
{
    {
        Node* tmp = rhv.m_head;
        rhv.m_head = m_head;
        m_head = tmp;
    }
    {
        auto tmp = rhv.m_lst_size;
        rhv.m_lst_size = m_lst_size;
        m_lst_size = tmp;
    }
}

SingleLinkedList::~SingleLinkedList()
{
    clear();
}

SingleLinkedList::Iter SingleLinkedList::find(SingleLinkedList::Predicate pred) const
{
    Node* cur = m_head;
    if (pred) // Test for functor nullptr
    {
        while (cur)
        {
            if (pred(cur->payload))
                return Iter{cur};
            cur = cur->next;
        }
    }
    return Iter{nullptr};
}

void SingleLinkedList::push_front(const SingleLinkedList::Payload& data)
{
    Node* new_node = new Node(data);
    Node* tmp = m_head;
    m_head = new_node;
    new_node->next = tmp;
    ++m_lst_size;
}

void SingleLinkedList::push_back(const SingleLinkedList::Payload& data)
{
    Node* new_node = new Node(data);

    if (m_head == nullptr)
        m_head = new_node;
    else
    {
        Node* curr = m_head;
        while(curr->next)
            curr = curr->next;

        curr->next = new_node;
    }

    ++m_lst_size;
}

//void SingleLinkedList::insert_after();

void SingleLinkedList::erase_after(SingleLinkedList::ConstIter elem)
{
    assert(elem != Iter{nullptr});
    Node* curr = elem.m_it.m_current;
    if (curr->next != nullptr)
    {
        Node* tmp = curr->next;
        curr->next = curr->next->next;
        --m_lst_size;
        delete tmp;
    }
}

bool operator == (const SingleLinkedList& lhv, const SingleLinkedList& rhv)
{
    if (lhv.get_size() != rhv.get_size())
        return false;

    SingleLinkedList::ConstIter lhv_curr = lhv.begin();
    SingleLinkedList::ConstIter lhv_last = lhv.end();
    SingleLinkedList::ConstIter rhv_curr = rhv.begin();

    while (lhv_curr != lhv_last && *lhv_curr++ == *rhv_curr++)
        ;

    return lhv_curr == lhv_last;
}

std::ostream& operator << (std::ostream& os, const SingleLinkedList& lst)
{
    for (auto it = lst.begin(), last = lst.end(); it != last; ++it)
        std::cout << "ID: " << (*it).id << " Name: " << (*it).name << '\n';

    return os;
}

} // namespace borealis
