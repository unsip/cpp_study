#include "BaseVisitor.hpp"

void BaseVisitor::visit(Attacker*, Defender*, Applier&) const {}

void BaseVisitor::visit(IRat& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(Rat& v) const { visit(static_cast<IRat&>(v)); }

void BaseVisitor::visit(ArmoredRat& v) const { visit(static_cast<IRat&>(v)); }

void BaseVisitor::visit(PlagueRat& v) const { visit(static_cast<IRat&>(v)); }

void BaseVisitor::visit(Hulk& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(Mimic& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(ZombieMimic& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(Slime& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(SlimeShard& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(SlimeQueen& v) const { visit(&v, &v, v); }

void BaseVisitor::visit(PoisonCloud& v) const { visit(&v, nullptr, v); }

void BaseVisitor::visit(Door& v) const { visit(nullptr, &v, v); }

void BaseVisitor::visit(StonePortal& v) const { visit(static_cast<Door&>(v)); }

