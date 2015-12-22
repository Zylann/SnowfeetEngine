#ifndef __HEADER_SN_COMPONENTMANAGER__
#define __HEADER_SN_COMPONENTMANAGER__

#include <core/scene/Component.h>
#include <core/scene/Entity.h>

namespace sn
{

// One manager for one component type. Managers implement this interface.
class SN_API IComponentManager : public Object
{
public:
    SN_OBJECT

    IComponentManager();
    virtual ~IComponentManager() {}

    void setScene(Scene & scene);
    Scene & getScene();

    /// \brief Gets which component class this manager works with
    virtual const ObjectType & getComponentType() const = 0;

    /// \brief Adds a new component on the entity. The component can be a recycled version of an older one.
    /// If a component of this type is already present on the entity, returns one of them.
    virtual Component * addComponent(EntityID id) = 0;

    /// \brief Gets a component from the given entity.
    /// Returns null if not found.
    virtual Component * getComponent(EntityID id) const = 0;

    /// \brief Removes a component from the given entity.
    /// Returns true if it was removed, false if not found.
    virtual bool removeComponent(EntityID id) = 0;

    virtual void onUpdate() {}

private:
    Scene * r_scene;

};

} // namespace sn

#endif // __HEADER_SN_COMPONENTMANAGER__


