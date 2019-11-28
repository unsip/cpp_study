#include "DeathVisitor.hpp"
#include "EventDecorator.hpp"
#include "IEventDispatcher.hpp"

void DeathVisitor::visit(Attacker* a, Defender* d, Applier& app) const
{
    m_ed.on_terminate_emit(a, d, app);
}

void DeathVisitor::visit(ZombieMimic& v) const
{
    if (!v.reborn())
        m_ed.on_terminate_emit(&v, &v, v);
}

void DeathVisitor::visit(SlimeQueen& v) const
{
    auto brood = v.detach_shards();
    for (auto* p : brood)
        m_ed.on_create_emit(p, new EventDecorator(p, p, *p, m_ed), *p);

    m_ed.on_terminate_emit(&v, &v, v);
}
