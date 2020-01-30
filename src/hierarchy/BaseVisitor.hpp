#pragma once

#include "IBestiaryVisitor.hpp"

class BaseVisitor : public IBestiaryVisitor
{
public:
    void visit(Attacker*, Defender*, Applier&) const override;

    void visit(IRat&) const override;
    void visit(Rat&) const override;
    void visit(ArmoredRat& v) const override;
    void visit(PlagueRat& v) const override;

    void visit(Hulk&) const override;
    void visit(Mimic&) const override;
    void visit(ZombieMimic&) const override;

    void visit(Slime&) const override;
    void visit(SlimeShard&) const override;
    void visit(SlimeQueen&) const override;

    void visit(PoisonCloud&) const override;
    void visit(Door&) const override;
    void visit(StonePortal& v) const override;
};
