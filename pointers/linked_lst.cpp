/*
 *  Singly-linked list implementation.
 */
#include <iostream>
#include <cassert>
#include <random>

namespace borealis
{

class SingleLinkedList
{
public:
    struct Payload
    {
        int id;
        std::string name;
    };

    typedef bool(* Predicate)(const Payload&);

private:
    struct Node
    {
        Node(const Payload& initial):
            payload(initial), next(nullptr)
        {}
        Payload payload;
        Node* next;
    };
    Node* m_head = nullptr;
    std::size_t m_lst_size = 0;

public:
    class Iter;
    class ConstIter;

    SingleLinkedList() = default;   // Use default constructor
    SingleLinkedList(const Payload&, std::size_t);
    template <typename InputIterator1, typename InputIterator2>
    SingleLinkedList(InputIterator1, InputIterator2);

    ~SingleLinkedList();

    Iter begin();
    Iter end();
    ConstIter begin() const;
    ConstIter end() const;

    std::size_t get_size() const
    {
        return m_lst_size;
    }

    Iter find(Predicate) const;
    void push_front(const Payload&);
    void push_back(const Payload&);
    void erase_after(ConstIter);
}; // class SingleLinkedList


class SingleLinkedList::Iter
{
private:
    Node* m_current;
    friend bool operator == (const SingleLinkedList::Iter&, const SingleLinkedList::Iter&);
    friend class SingleLinkedList;

    explicit Iter(Node* current)        // Allow only explicit cast (f.e. void foo(Iter); foo(Node*) - will fail)
        : m_current(current)
    {}

public:
    Iter& operator ++ ()                // Pre-increment
    {
        if (m_current != nullptr)
            m_current = m_current->next;

        return *this;
    }

    Iter operator ++ (int)              // Post-increment
    {
        Iter current{*this};
        ++*this;

        return current;
    }

    Payload& operator * ()              // Mutating operator (for non-constant *this*)
    {
        assert(m_current != nullptr && "Illegal dereference of singular iterator!");
        return m_current->payload;
    }

    const Payload& operator * () const  // Immutating operator (for non-constant *this*)
    {
        assert(m_current != nullptr && "Illegal dereference of singular iterator!");
        return m_current->payload;
    }

}; // class SingleLinkedList::Iter

class SingleLinkedList::ConstIter
{
private:
    Iter m_it;
    friend bool operator == (const SingleLinkedList::ConstIter&, const SingleLinkedList::ConstIter&);
    friend class SingleLinkedList;

public:
    ConstIter(Iter it)
        : m_it{it}
    {}

    ConstIter& operator ++ ()               // Pre-increment
    {
        ++m_it;
        return *this;
    }

    ConstIter operator ++ (int)             // Post-increment
    {
        ConstIter current{*this};
        ++*this;

        return current;
    }

    const Payload& operator * () const // Constant operator (for constant *this*, e.g. 'const SingleLinkedList::Iter')
    {
        return *m_it;
    }
}; // class SingleLinkedList::ConstIter


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

SingleLinkedList::SingleLinkedList(const SingleLinkedList::Payload& initial, std::size_t count)
{
    Node* current = m_head;

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

    m_lst_size = count;
}

// Templating two iterators, coz legal cases are:
// 1. Iter, Iter.
// 2. ConstIter, ConstIter.
// 3. Iter, ConstIter.
// 4. ConstIter, Iter.
template <typename InputIterator1, typename InputIterator2>
SingleLinkedList::SingleLinkedList(InputIterator1 first, InputIterator2 last)
{
    Node* current = m_head;

    while (first != last)
    {
        // TODO: Exceptions. Transaction semantics.
        Node* new_elem = new Node(*first);
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
        ++first;
        ++m_lst_size;
    }
}

SingleLinkedList::~SingleLinkedList()
{
    Node* current = m_head;

    while (current)
    {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
}

bool operator == (const SingleLinkedList::Iter& lhv, const SingleLinkedList::Iter& rhv)
{
    return lhv.m_current == rhv.m_current;
}

bool operator == (const SingleLinkedList::ConstIter& lhv, const SingleLinkedList::ConstIter& rhv)
{
    return lhv.m_it == rhv.m_it;
}

bool operator != (const SingleLinkedList::ConstIter& lhv, const SingleLinkedList::ConstIter& rhv)
{
    return !(lhv == rhv);
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
    // TODO: Error handling on malloc or use smart pointers
    Node* new_node = new Node(data);
    Node* tmp = m_head;
    m_head = new_node;
    new_node->next = tmp;
    ++m_lst_size;
}

void SingleLinkedList::push_back(const SingleLinkedList::Payload& data)
{
    // TODO: Error handling on malloc or use smart pointers
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
        curr->next = curr->next->next;
        --m_lst_size;
        delete curr->next;
    }
}

std::ostream& operator << (std::ostream& os, const SingleLinkedList& lst)
{
    for (auto it = lst.begin(), last = lst.end(); it != last; ++it)
        std::cout << "ID: " << (*it).id << " Name: " << (*it).name << '\n';

    return os;
}

} // namespace borealis

int main()
{
    std::string names_choice [] = {"Andrey", "Kolya", "Vadim", "Costas", "Leysan", "Olli"};

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> dis(0, sizeof names_choice / sizeof(std::string) - 1);

    borealis::SingleLinkedList my_lst;

    for (int i = 0; i < 20; ++i)
    {
        borealis::SingleLinkedList::Payload new_payload{i, names_choice[dis(gen)]};
        my_lst.push_front(new_payload);
    }

    borealis::SingleLinkedList empty_lst;

    for (const auto& val : my_lst)
        empty_lst.push_front(val);

    std::cout << my_lst << std::endl;
    std::cout << empty_lst << std::endl;

    borealis::SingleLinkedList inherited_lst{my_lst.begin(), my_lst.end()};
    std::cout << inherited_lst << std::endl;

    borealis::SingleLinkedList copied_lst{empty_lst};
}
