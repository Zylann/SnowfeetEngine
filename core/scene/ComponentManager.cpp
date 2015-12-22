#include "ComponentManager.h"

namespace sn
{

SN_OBJECT_IMPL(IComponentManager)

IComponentManager::IComponentManager() : r_scene(nullptr)
{
}

void IComponentManager::setScene(Scene & scene)
{
    SN_ASSERT(r_scene == nullptr, "Scene already set");
    r_scene = &scene;
}

Scene & IComponentManager::getScene()
{
    SN_ASSERT(r_scene != nullptr, "Scene is not set");
    return *r_scene;
}

} // namespace sn

