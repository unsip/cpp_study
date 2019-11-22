#pragma once

#include "Bestiary.hpp"

class IBestiaryVisitor
{
public:
    virtual void visit(Attacker*, Defender*, Applier&) const = 0;

    virtual void visit(Rat&) const = 0;
    virtual void visit(ArmoredRat&) const = 0;
    virtual void visit(PlagueRat&) const = 0;

    virtual void visit(Hulk&) const = 0;
    virtual void visit(Mimic&) const = 0;
    virtual void visit(ZombieMimic&) const = 0;

    virtual void visit(Slime&) const = 0;
    virtual void visit(SlimeShard&) const = 0;
    virtual void visit(SlimeQueen&) const = 0;

    virtual void visit(PoisonCloud&) const = 0;
    virtual void visit(Door&) const = 0;
    virtual void visit(StonePortal&) const = 0;
};
