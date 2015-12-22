#include "Component.h"
#include "Entity.h"

namespace sn
{

SN_OBJECT_IMPL(Component)

Component::Component() : r_entity(nullptr)
{
    m_flags[FLAG_ENABLED] = true;
}

Component::~Component()
{
}

Entity & Component::getEntity()
{
    SN_ASSERT(r_entity, "Component's entity is null");
    return *r_entity;
}

void Component::setEntity(Entity & e)
{
    SN_ASSERT(r_entity == nullptr, "Component's entity cannot be set twice");
    r_entity = &e;
}

void Component::serializeState(Variant & o, const SerializationContext & context)
{
}

void Component::unserializeState(const Variant & o, const SerializationContext & context)
{
}

} // namespace sn

