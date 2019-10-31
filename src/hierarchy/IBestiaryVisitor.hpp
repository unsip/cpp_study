#pragma once

#include "Bestiary.hpp"

class IBestiaryVisitor
{
public:
    virtual void visit(const Rat&) const = 0;
    virtual void visit(const ArmoredRat&) const = 0;
    virtual void visit(const PlagueRat&) const = 0;

    virtual void visit(const Hulk&) const = 0;
    virtual void visit(const Mimic&) const = 0;

    virtual void visit(const Slime&) const = 0;
    virtual void visit(const SlimeShard&) const = 0;
    virtual void visit(const SlimeQueen&) const = 0;

    virtual void visit(const PoisonCloud&) const = 0;
    virtual void visit(const Door&) const = 0;
    virtual void visit(const StonePortal&) const = 0;
};
