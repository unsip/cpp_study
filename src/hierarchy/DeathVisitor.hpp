#pragma once

#include "BaseVisitor.hpp"

class DeathVisitor : public BaseVisitor
{
public:
    explicit DeathVisitor();
    void visit(const ZombieMimic&) const override;
    void visit(const SlimeQueen&) const override;

private:
    std::function<void(Defender*, Attacker*)> m_add2scene;
};

