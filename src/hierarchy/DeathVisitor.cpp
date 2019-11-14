#include "DeathVisitor.hpp"


// TODO: Refactor with dependency injection of Scene.
DeathVisitor::DeathVisitor(std::function<void()> add2scene)
  : m_add2scene(std::move(add2scene))
{
}

void DeathVisitor::visit(const ZombieMimic& v) const
{
    if (!v.reborn())
        m_erase_from_scene(v);
}

void DeathVisitor::visit(const SlimeQueen& v) const
{
    auto brood = v.detach_shards();
    for (auto* p : brood)
        m_add2scene(p, p);
}
