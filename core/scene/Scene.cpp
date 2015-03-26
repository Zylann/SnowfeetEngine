/*
Scene.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Scene.hpp"

namespace sn
{

//------------------------------------------------------------------------------
void Scene::registerUpdatableEntity(Entity & e, s16 order, s16 layer)
{
    s32 packedOrder = static_cast<s32>(order) | (static_cast<s32>(layer) << 16);
#ifdef SN_BUILD_DEBUG
    auto it = m_updatableEntities.find(packedOrder);
    if (it != m_updatableEntities.end())
    {
        if (it->second.find(&e) != it->second.end())
        {
            SN_ERROR("Scene::registerUpdatableEntity: entity " << e.toString() << " is already registered with order " << order << " and layer " << layer);
            return;
        }
    }
#endif
    m_updatableEntities[packedOrder].insert(&e);
}

//------------------------------------------------------------------------------
void Scene::unregisterUpdatableEntity(Entity & e)
{
    for (auto it = m_updatableEntities.begin(); it != m_updatableEntities.end(); ++it)
    {
        if (it->second.erase(&e) != 0)
            return;
    }
#ifdef SN_BUILD_DEBUG
    SN_WARNING("Scene::unregisterUpdatableEntity: entity " << e.toString() << " was not registered");
#endif
}

//------------------------------------------------------------------------------
void Scene::registerEventListener(Entity & e)
{
	auto it = m_eventListenerEntities.find(&e);
	if (it == m_eventListenerEntities.end())
	{
		m_eventListenerEntities.insert(&e);
	}
	else
	{
		SN_ERROR("Scene::registerEventListener: entity " << e.toString() << " is already registered");
	}
}

//------------------------------------------------------------------------------
void Scene::unregisterEventListener(Entity & e)
{
	if (m_eventListenerEntities.erase(&e) == 0)
	{
	    SN_WARNING("Scene::unregisterEventListener: entity " << e.toString() << " was not registered");
	}
}

//------------------------------------------------------------------------------
void Scene::registerTaggedEntity(Entity & e, const std::string & tag)
{
#ifdef SN_BUILD_DEBUG
    auto it = m_taggedEntities.find(tag);
    if (it != m_taggedEntities.end())
    {
        if (it->second.find(&e) != it->second.end())
        {
            SN_ERROR("Scene::registerTaggedEntity: entity " << e.toString() << " already registered with tag " << tag);
            return;
        }
    }
#endif
    m_taggedEntities[tag].insert(&e);
}

//------------------------------------------------------------------------------
void Scene::unregisterTaggedEntity(Entity & e, const std::string & tag)
{
    if (m_taggedEntities[tag].erase(&e) == 0)
        SN_WARNING("Scene::unregisterTaggedEntity: entity " << e.toString() << " was not registered with tag " << tag);
}

//------------------------------------------------------------------------------
Entity * Scene::getTaggedEntity(const std::string & tag)
{
    auto it = m_taggedEntities.find(tag);
    if (it != m_taggedEntities.end())
    {
        if (it->second.empty())
            return nullptr;
        else
            return *(it->second.begin());
    }
    else
    {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
std::vector<Entity*> Scene::getTaggedEntities(const std::string & tag)
{
    std::vector<Entity*> entities;
    auto it = m_taggedEntities.find(tag);
    if (it != m_taggedEntities.end())
    {
        if (!it->second.empty())
        {
            const auto & taggedEntities = it->second;
            for (auto it2 = taggedEntities.begin(); it2 != taggedEntities.end(); ++it2)
                entities.push_back(*it2);
        }
    }
    return entities;
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
void Scene::update(Time deltaTime)
{
	m_deltaTime = deltaTime;
	onUpdate();
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
bool Scene::onSystemEvent(const Event & ev)
{
    // TODO EventDispatcher

	auto listenersCopy = m_eventListenerEntities;
	for (auto it = listenersCopy.begin(); it != listenersCopy.end(); ++it)
	{
		if ((*it)->onSystemEvent(ev))
			return true; // Handled
	}

    return false; // Not handled
}

//------------------------------------------------------------------------------
void Scene::destroyChildrenButServices()
{
    destroyChildren([](const Entity & e){
        return !e.getFlag(SN_EF_STICKY);
    });
}

//------------------------------------------------------------------------------
Time Scene::getTimeSinceStartup() const
{
	return m_timeClock.getElapsedTime();
}

//------------------------------------------------------------------------------
void Scene::loadFromFile(const std::string & filePath, const SerializationContext & context)
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
        // Note: the returned child will be automatically added to the children list,
        // as soon as setParent() is called
        Entity::unserialize(docEntities[i], this, context);
    }
}

//------------------------------------------------------------------------------
void Scene::saveToFile(const std::string & filePath, const SerializationContext & context)
{
    JsonBox::Value doc;
    JsonBox::Value & docEntities = doc["entities"];
    for (u32 i = 0; i < getChildCount(); ++i)
    {
        Entity::serialize(docEntities[i], *getChildByIndex(i), context);
    }

    sn::saveToFile(doc, filePath);
}


} // namespace sn

