#ifndef __HEADER_SN_COMPONENTSYSTEM__
#define __HEADER_SN_COMPONENTSYSTEM__

#include <core/scene/ComponentManager.h>

namespace sn
{

// Where all component managers are stored, usually in the Scene class itself.
// If you get the scene, you access components.
class SN_API ComponentSystem
{
public:
    void createManagers();

    Component * addComponent(const ObjectType & cmpType, EntityID eid);
    Component * getComponent(const ObjectType & cmpType, EntityID eid) const;
    bool removeComponent(const ObjectType & cmpType, EntityID id);

    void getAllComponentsOnEntity(EntityID id, std::vector<Component*> & out_components);

    void removeAllComponentsOnEntity(EntityID id);

    void update();

    ~ComponentSystem();

private:
    IComponentManager * getManagerFor(const ObjectType & cmpType) const;
    void addManager(IComponentManager * manager);
    bool isManagerRegistered(const ObjectType & cmpType) const;

private:
    // Managers associated by component type.
    // We can handle inheritance by using reflection when managers are added, and insert pointers several times.
    std::vector<IComponentManager*> m_managers;
};

} // namespace sn

#endif // __HEADER_SN_COMPONENTSYSTEM__

