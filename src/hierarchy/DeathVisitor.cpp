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
    for (std::unique_ptr<SlimeShard>& p : brood)
    {
        std::shared_ptr<SlimeShard> t(std::move(p));
        m_ed.on_create_emit(
            t
          , std::shared_ptr<Defender>(new EventDecorator(t, t, t, m_ed))
          , *t
        );
    }

    m_ed.on_terminate_emit(&v, &v, v);
}
