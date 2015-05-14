#ifndef __HEADER_SN_COMPONENT__
#define __HEADER_SN_COMPONENT__

#include "Entity.hpp"

namespace sn
{

//------------------------------------------------------------------------------
// An entity is just a number
typedef u32 EntityID;

//------------------------------------------------------------------------------
// Base class for components
class SN_API Component : public ScriptObject
{
public:
    SN_SCRIPT_OBJECT(sn::Component, sn::ScriptObject)

    Component(Entity & e) : r_entity(e) {}

private:
    virtual ~Component();

private:
    Entity & r_entity;
};

//------------------------------------------------------------------------------
// One manager for one component type. Managers implement this interface.
class SN_API IComponentManager : public Object
{
public:
    SN_OBJECT(sn::IComponentManager, sn::Object)

    // Gets which component class this manager works with
    virtual const ObjectType & getComponentType() const = 0;

    // Creates a new component on the given entity
    virtual Component * addComponent(EntityID id) = 0;
    virtual Component * getComponent(EntityID id) const = 0;
    virtual bool removeComponent(EntityID id) = 0;

protected:
    virtual ~IComponentManager() {}
};

//------------------------------------------------------------------------------
// Generic implementation of a component manager
class SN_API GenericComponentManager : public IComponentManager
{
public:
    SN_OBJECT(sn::GenericComponentManager, sn::IComponentManager)

    virtual const ObjectType & getComponentType() const override;

    virtual Component * addComponent(EntityID id) override;
    virtual Component * getComponent(EntityID id) const override;
    virtual bool removeComponent(EntityID id) override;

protected:
    virtual ~GenericComponentManager() {}

protected:
    // TODO The reason for this mapping is we are not sure unordered_map is a contiguous container,
    // because the standard states that pointers to elements in it must be stable (they aren't in vectors)
    std::vector<Component*> m_components;
    std::unordered_map<EntityID, u32> m_mapping;
};

//------------------------------------------------------------------------------
// Where all component managers are stored, usually in the Scene class itself.
// If you get the scene, you access components.
class SN_API ComponentSystem
{
public:
    void addManager(IComponentManager * mgr);
    void removeManager(IComponentManager * mgr);
    void getManagers(const ObjectType & cmpType, std::vector<IComponentManager*> & out_managers, bool includeInheritance);

    Component * addComponent(const ObjectType & cmpType, EntityID eid);
    Component * getComponent(const ObjectType & cmpType, EntityID eid) const;
    virtual bool removeComponent(const ObjectType & cmpType, EntityID id);

    void getAllComponentsOnEntity(EntityID id, std::vector<Component*> & out_components);

private:
    // Managers associated by component type.
    // We can handle inheritance by using reflection when managers are added, and insert pointers several times.
    std::unordered_map<ObjectTypeID, IComponentManager*> m_managers;
};

} // namespace sn

#endif // __HEADER_SN_COMPONENT__


