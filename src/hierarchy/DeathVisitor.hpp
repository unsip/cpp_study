#pragma once

#include "BaseVisitor.hpp"

class DeathVisitor : public BaseVisitor
{
private:
    IEventDispatcher& m_ed;
public:
    explicit DeathVisitor(IEventDispatcher& ed) noexcept
        : m_ed(ed)
    {}
    void visit(Attacker*, Defender*, Applier&) const override;
    void visit(ZombieMimic&) const override;
    void visit(SlimeQueen&) const override;
};

