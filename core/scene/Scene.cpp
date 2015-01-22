#include "Scene.hpp"

namespace sn
{

//------------------------------------------------------------------------------
void Scene::registerUpdatableEntity(Entity * e, s16 order, s16 layer)
{
    s32 packedOrder = static_cast<s32>(order) | (static_cast<s32>(layer) << 16);
#ifdef SN_BUILD_DEBUG
    auto it = m_updatableEntities.find(packedOrder);
    if (it != m_updatableEntities.end())
    {
        if (it->second.find(e) != it->second.end())
        {
            SN_ERROR("Scene::registerUpdatableEntity: entity " << e->toString() << " already registered with order " << order << " and layer " << layer);
            return;
        }
    }
#endif
    m_updatableEntities[packedOrder].insert(e);
}

//------------------------------------------------------------------------------
void Scene::unregisterUpdatableEntity(Entity * e)
{
    for (auto it = m_updatableEntities.begin(); it != m_updatableEntities.end(); ++it)
    {
        if (it->second.erase(e) != 0)
            return;
    }
#ifdef SN_BUILD_DEBUG
    SN_WARNING("Scene::underisterUpdatableEntity: entity " << e->toString() << " was not registered");
#endif
}

//------------------------------------------------------------------------------
void Scene::registerTaggedEntity(Entity * e, const std::string & tag)
{
#ifdef SN_BUILD_DEBUG
    auto it = m_taggedEntities.find(tag);
    if (it != m_taggedEntities.end())
    {
        if (it->second.find(e) != it->second.end())
        {
            SN_ERROR("Scene::registerTaggedEntity: entity " << e->toString() << " already registered with tag " << tag);
            return;
        }
    }
#endif
    m_taggedEntities[tag].insert(e);
}

//------------------------------------------------------------------------------
void Scene::unregisterTaggedEntity(Entity * e, const std::string & tag)
{
    if (m_taggedEntities[tag].erase(e) == 0)
        SN_WARNING("Scene::unregisterTaggedEntity: entity " << e->toString() << " was not registered with tag " << tag);
}

//------------------------------------------------------------------------------
void Scene::setParent(Entity * newParent)
{
    SN_ERROR("Scene::setParent: a scene cannot have a parent.");
}

//------------------------------------------------------------------------------
void Scene::destroy()
{
    SN_ERROR("Scene::destroy: a scene cannot be destroyed. Use destroyChildren() instead.");
}

//------------------------------------------------------------------------------
void Scene::destroyLater()
{
    SN_ERROR("Scene::destroyLater: a scene cannot be destroyed. Use destroyChildren() instead.");
}

//------------------------------------------------------------------------------
void Scene::onUpdate()
{
    auto sortedEntities = m_updatableEntities; // Iterate over a copy
    for (auto it = sortedEntities.begin(); it != sortedEntities.end(); ++it)
    {
        auto & entities = it->second;
        for (auto it2 = entities.begin(); it2 != entities.end(); ++it2)
        {
            Entity & e = **it2;
            if (!e.getFlag(SN_EF_FIRST_UPDATE))
            {
                e.onFirstUpdate();
                e.setFlag(SN_EF_FIRST_UPDATE, true);
            }
            e.onUpdate();
        }
    }

	// TODO Destroy entities with flag DESTROYED set
}

//------------------------------------------------------------------------------
void Scene::loadFromFile(const std::string & filePath)
{
    JsonBox::Value doc;
    if (!sn::loadFromFile(doc, filePath, -1, true))
    {
        return;
    }

    JsonBox::Value & docEntities = doc["entities"];
    u32 len = docEntities.getArray().size();
    for (u32 i = 0; i < len; ++i)
    {
        Entity::unserialize(docEntities[i], *this);
    }
}

//------------------------------------------------------------------------------
void Scene::saveToFile(const std::string & filePath)
{
    JsonBox::Value doc;
    JsonBox::Value & docEntities = doc["entities"];
    for (u32 i = 0; i < getChildCount(); ++i)
    {
        Entity::serialize(docEntities[i], *getChildByIndex(i));
    }

    sn::saveToFile(doc, filePath);
}


} // namespace sn

