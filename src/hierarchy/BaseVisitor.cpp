#include "BaseVisitor.hpp"

void BaseVisitor::visit(const Rat&) const {}

void BaseVisitor::visit:(const ArmoredRat& v) const
{
    visit(static_cast<const Rat&>(v));
}

void BaseVisitor::visit(const PlagueRat& v) const
{
    visit(static_cast<const Rat&>(v));
}

void BaseVisitor::visit(const Hulk&) const {}

void BaseVisitor::visit(const Mimic&) const {}

void BaseVisitor::visit(const ZombieMimic&) const {}

void BaseVisitor::visit(const Slime&) const {}

void BaseVisitor::visit(const SlimeShard&) const {}

void BaseVisitor::visit(const SlimeQueen&) const {}

void BaseVisitor::visit(const PoisonCloud&) const {}

void BaseVisitor::visit(const Door&) const {}

void BaseVisitor::visit(const StonePortal& v) const
{
    visit(static_cast<const Door&>(v));
}

