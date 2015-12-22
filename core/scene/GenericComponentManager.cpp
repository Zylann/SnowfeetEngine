#include "GenericComponentManager.h"
#include <core/util/typecheck.h>

namespace sn
{

SN_OBJECT_IMPL(GenericComponentManager)

GenericComponentManager::GenericComponentManager() : r_objectType(nullptr)
{
}

GenericComponentManager::~GenericComponentManager()
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        Component * cmp = *it;
        if (cmp)
            cmp->release();
    }
    releaseRemovedComponents();
}

void GenericComponentManager::setComponentType(const ObjectType & ot)
{
    SN_ASSERT(r_objectType == nullptr, "Cannot set component type twice");
    SN_ASSERT(!ot.isAbstract(), "Object type must be concrete");
    SN_ASSERT(ot.derivesFrom(sn::getObjectType<Component>()), "Object type must derive from Component");
    r_objectType = &ot;
}

const ObjectType & GenericComponentManager::getComponentType() const
{
    SN_ASSERT(r_objectType != nullptr, "Component type is not set");
    return *r_objectType;
}

Component * GenericComponentManager::addComponent(EntityID id)
{
    Component * cmp = getComponent(id);
    if (cmp)
        return cmp;
    if (id.i >= m_components.size())
        m_components.resize(id.i + 1, nullptr);
    cmp = checked_cast<Component*>(r_objectType->instantiate());
    m_components[id.i] = cmp;
    return cmp;
}

Component * GenericComponentManager::getComponent(EntityID id) const
{
    u32 i = id.i;
    if (i < m_components.size())
        return m_components[i];
    return nullptr;
}

bool GenericComponentManager::removeComponent(EntityID id)
{
    u32 i = id.i;
    if (i < m_components.size())
    {
        Component * cmp = m_components[i];
        if (cmp)
        {
            m_removedComponents.push_back(cmp);
            m_components[i] = nullptr;
            return true;
        }
    }
    return false;
}

void GenericComponentManager::releaseRemovedComponents()
{
    for (auto it = m_removedComponents.begin(); it != m_removedComponents.end(); ++it)
    {
        Component * cmp = *it;
        cmp->release();
    }
    m_removedComponents.clear();
}

void GenericComponentManager::onUpdate()
{
    releaseRemovedComponents();
}

} // namespace sn

