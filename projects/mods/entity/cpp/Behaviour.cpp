#include "Scene.hpp"
#include "Behaviour.hpp"

namespace sn
{

SN_OBJECT_IMPL(sn::Behaviour)

void Behaviour::onAdd(Entity * e)
{
    Component::onAdd(e);
    entity().scene().registerBehaviour(this);
}

void Behaviour::onDestroy()
{
    entity().scene().unregisterBehaviour(this);
}

} // namespace sn

