#pragma once

#include "BaseVisitor.hpp"
#include "IEventDispatcher.hpp"


class DamageVisitor : public BaseVisitor
{
private:
    IEventDispatcher& m_ed;
    std::size_t m_dmg;
public:
    explicit DamageVisitor(IEventDispatcher& ed, std::size_t dmg) noexcept
        : m_ed(ed), m_dmg(dmg)
    {}
    void visit(SlimeQueen&) const override;
};
