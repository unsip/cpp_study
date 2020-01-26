#pragma once

#include "BaseVisitor.hpp"
#include "IEventDispatcher.hpp"

class DeathVisitor : public BaseVisitor
{
private:
    Defender* m_def;
    IEventDispatcher& m_ed;
public:
    explicit DeathVisitor(Defender* def, IEventDispatcher& ed) noexcept
        : m_def(def), m_ed(ed)
    {}
    void visit(Attacker*, Defender*, Applier&) const override;
    void visit(ZombieMimic&) const override;
    void visit(SlimeQueen&) const override;
};

