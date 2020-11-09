#pragma once

#include <cassert>
#include <string>

namespace borealis {
// TODO: Generalize and move to phase library.
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
    static Node* copy(const Node*);
    static void clear(const Node*) noexcept;

public:
    class Iter;
    class ConstIter;
    SingleLinkedList() = default;   // Use default constructor
    SingleLinkedList(const Payload&, std::size_t);
    template <typename InputIterator1, typename InputIterator2>
    SingleLinkedList(InputIterator1, InputIterator2);
    SingleLinkedList(const SingleLinkedList&);
    SingleLinkedList& operator = (SingleLinkedList);

    ~SingleLinkedList();

    Iter begin();
    Iter end();
    ConstIter begin() const;
    ConstIter end() const;

    std::size_t get_size() const
    {
        return m_lst_size;
    }

    void clear() noexcept;
    Iter find(Predicate) const;
    void push_front(const Payload&);
    void push_back(const Payload&);
    void erase_after(ConstIter);
    void swap(SingleLinkedList&) noexcept;

private:
    Node* m_head = nullptr;
    std::size_t m_lst_size = 0;

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

// Templating two iterators, coz legal cases are:
// 1. Iter, Iter.
// 2. ConstIter, ConstIter.
// 3. Iter, ConstIter.
// 4. ConstIter, Iter.
template <typename InputIterator1, typename InputIterator2>
inline SingleLinkedList::SingleLinkedList(InputIterator1 first, InputIterator2 last)
{
    Node* current = m_head;

    try
    {
        while (first != last)
        {
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
    catch (...)
    {
        clear();
        throw;
    }
}

inline bool operator == (const SingleLinkedList::Iter& lhv, const SingleLinkedList::Iter& rhv)
{
    return lhv.m_current == rhv.m_current;
}

inline bool operator == (const SingleLinkedList::ConstIter& lhv, const SingleLinkedList::ConstIter& rhv)
{
    return lhv.m_it == rhv.m_it;
}

inline bool operator != (const SingleLinkedList::ConstIter& lhv, const SingleLinkedList::ConstIter& rhv)
{
    return !(lhv == rhv);
}

inline bool operator == (const SingleLinkedList::Payload& lhv, const SingleLinkedList::Payload& rhv)
{
    return lhv.id == rhv.id && lhv.name == rhv.name;
}

bool operator == (const SingleLinkedList& lhv, const SingleLinkedList& rhv);

inline bool operator != (const SingleLinkedList& lhv, const SingleLinkedList& rhv)
{
    return !(lhv == rhv);
}

std::ostream& operator << (std::ostream& os, const SingleLinkedList& lst);

} // namespace borealis
