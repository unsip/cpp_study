#include "DamageVisitor.hpp"
#include "EventDecorator.hpp"
#include "IEventDispatcher.hpp"

#include <random>

void DamageVisitor::visit(SlimeQueen& v) const
{
    static std::random_device rd;
    static std::mt19937 gen{rd()};
    static std::uniform_int_distribution<unsigned> d3(1, 3);
    static std::uniform_int_distribution<unsigned> d6(1, 6);

    if (v.is_dead())
    {
        v.spawn_shards(m_dmg);
    }
    else
    {
        if (d6(gen) == 6)
        {
            // SlimeQueen splits in two
            SlimeQueen* q = new SlimeQueen(v.get_hp() / 2, v.attack(), m_ed);
            v.set_hp(v.get_hp() / 2);
            m_ed.on_create_emit(q, new EventDecorator(q, q, *q, m_ed), *q);
        }
        else if (d3(gen) == 3)
        {
            SlimeShard* s = new SlimeShard(m_dmg, m_ed);
            m_ed.on_create_emit(s, new EventDecorator(s, s, *s, m_ed), *s);
        }
    }
}
