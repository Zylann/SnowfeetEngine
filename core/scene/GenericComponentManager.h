#ifndef __HEADER_SN_GENERICCOMPONENTMANAGER__
#define __HEADER_SN_GENERICCOMPONENTMANAGER__

#include <core/scene/ComponentManager.h>

namespace sn
{

// Generic implementation of a component manager
class SN_API GenericComponentManager : public IComponentManager
{
public:
    SN_OBJECT

    GenericComponentManager();

    void setComponentType(const ObjectType & ot);

    virtual const ObjectType & getComponentType() const override;

    virtual Component * addComponent(EntityID id) override;
    virtual Component * getComponent(EntityID id) const override;
    virtual bool removeComponent(EntityID id) override;

    virtual void onUpdate() override;

protected:
    virtual ~GenericComponentManager();

    void releaseRemovedComponents();

protected:
    std::vector<Component*> m_components;
    std::vector<Component*> m_removedComponents;
    const ObjectType * r_objectType;

};

} // namespace sn

#endif // __HEADER_SN_GENERICCOMPONENTMANAGER__

