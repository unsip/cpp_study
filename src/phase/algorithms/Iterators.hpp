#pragma once

#include <iterator>

// TODO: Implement std::make_reverse_iterator
namespace phase::iterators {
template <typename It>
class ReverseIterator
{
private:
    It m_it;
public:
    explicit ReverseIterator(It it) : m_it(it) {}
    ReverseIterator& operator ++() { --m_it; return *this; }
    ReverseIterator operator ++(int) { return ReverseIterator(m_it--); }
    typename std::iterator_traits<It>::value_type& operator *() { It tmp = m_it; return *--tmp; }
    bool operator ==(const ReverseIterator& rhv) const { return m_it == rhv.m_it; }
    bool operator !=(const ReverseIterator& rhv) const { return !(*this == rhv); }
    It base() const { return m_it; }
};
}

