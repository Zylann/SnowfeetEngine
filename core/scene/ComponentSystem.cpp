#include "ComponentSystem.h"
#include "GenericComponentManager.h"

namespace sn
{

void ComponentSystem::createManagers()
{
    const ObjectType & genericManagerT = sn::getObjectType<GenericComponentManager>();

    // Create specialized managers
    const ObjectType & baseManagerType = sn::getObjectType<IComponentManager>();
    const ObjectTypeList & types = ObjectTypeDatabase::get().getTypes();
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & ot = **it;
        if (!ot.isAbstract() && !ot.is(genericManagerT, false) && ot.derivesFrom(baseManagerType) && getManagerFor(ot) == nullptr)
        {
            IComponentManager * manager = checked_cast<IComponentManager*>(ot.instantiate());
            addManager(manager);
        }
    }

    // Create generic managers
    const ObjectType & baseComponentType = sn::getObjectType<Component>();
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & ot = **it;
        if (!ot.isAbstract() && ot.derivesFrom(baseComponentType) && getManagerFor(ot) == nullptr)
        {
            GenericComponentManager * genericManager = new GenericComponentManager();
            genericManager->setComponentType(ot);
            addManager(genericManager);
        }
    }
}

ComponentSystem::~ComponentSystem()
{
    for (auto it = m_managers.begin(); it != m_managers.end(); ++it)
    {
        IComponentManager * manager = *it;
        delete manager;
    }
    m_managers.clear();
}

void ComponentSystem::addManager(IComponentManager * manager)
{
    const ObjectType & componentType = manager->getComponentType();
    u32 i = componentType.getID();
    if (m_managers.size() < i)
        m_managers.resize(i + 1, nullptr);
    SN_ASSERT(m_managers[i] == nullptr, "Component manager already registered");
    m_managers[i] = manager;
}

IComponentManager * ComponentSystem::getManagerFor(const ObjectType & cmpType) const
{
    u32 i = cmpType.getID();
    if (i < m_managers.size())
        return m_managers[i];
    return nullptr;
}

Component * ComponentSystem::addComponent(const ObjectType & cmpType, EntityID eid)
{
    IComponentManager * manager = getManagerFor(cmpType);
    if (manager)
        return manager->addComponent(eid);
    return nullptr;
}

Component * ComponentSystem::getComponent(const ObjectType & cmpType, EntityID eid) const
{
    IComponentManager * manager = getManagerFor(cmpType);
    if (manager)
        return manager->getComponent(eid);
    return nullptr;
}

bool ComponentSystem::removeComponent(const ObjectType & cmpType, EntityID id)
{
    IComponentManager * manager = getManagerFor(cmpType);
    if (manager)
        return manager->removeComponent(id);
    return false;
}

void ComponentSystem::update()
{
    for (auto it = m_managers.begin(); it != m_managers.end(); ++it)
    {
        IComponentManager * manager = *it;
        if (manager)
        {
            manager->onUpdate();
        }
    }
}

void ComponentSystem::getAllComponentsOnEntity(EntityID id, std::vector<Component*> & out_components)
{
    for (auto it = m_managers.begin(); it != m_managers.end(); ++it)
    {
        IComponentManager * manager = *it;
        Component * component = manager->getComponent(id);
        if (component)
        {
            out_components.push_back(component);
        }
    }
}

void ComponentSystem::removeAllComponentsOnEntity(EntityID id)
{
    for (auto it = m_managers.begin(); it != m_managers.end(); ++it)
    {
        IComponentManager * manager = *it;
        if (manager)
        {
            manager->removeComponent(id);
        }
    }
}

} // namespace sn

