#pragma once

#include "IBestiaryVisitor.hpp"

class DescriptionOutput : public IBestiaryVisitor
{
public:
    void visit(const Rat&) const override;
    void visit(const ArmoredRat&) const override;
    void visit(const PlagueRat&) const override;

    void visit(const Hulk&) const override;
    void visit(const Mimic&) const override;

    void visit(const Slime&) const override;
    void visit(const SlimeShard&) const override;
    void visit(const SlimeQueen&) const override;

    void visit(const PoisonCloud&) const override;
    void visit(const Door&) const override;
    void visit(const StonePortal&) const override;
};
