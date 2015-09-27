/*
Scene.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Scene.h"

namespace sn
{

//------------------------------------------------------------------------------
const char * Scene::DEFAULT_UPDATE_TAG = "DefaultUpdate";

//------------------------------------------------------------------------------
Scene::Scene() : Entity(), 
    m_updateManager(),
    m_quitFlag(false)
{
    setName("Scene");
}

/*
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
bool Scene::getEntityUpdateOrder(Entity & e, s16 & out_order, s16 & out_layer) const
{
    for (auto it = m_updatableEntities.begin(); it != m_updatableEntities.end(); ++it)
    {
        const std::unordered_set<Entity*> & entities = it->second;
        if (entities.find(&e) != entities.end())
        {
            s32 n = it->first;
            out_order = n & 0x0000ffff;
            out_layer = (n >> 16) & 0x0000ffff;
            return true;
        }
    }
    return false;
}
*/

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
u32 Scene::registerTaggedEntity(Entity & e, const std::string & tag)
{
#ifdef SN_BUILD_DEBUG
    if (m_tagManager.isObjectTagged(&e, tag))
    {
        SN_ERROR("Scene::registerTaggedEntity: entity " << e.toString() << " already registered with tag " << tag);
        return m_tagManager.getTagIndex(tag);
    }
#endif
	return m_tagManager.add(tag, &e);
}

//------------------------------------------------------------------------------
u32 Scene::unregisterTaggedEntity(Entity & e, const std::string & tag)
{
	u32 tagIndex = m_tagManager.getTagIndex(tag);
	if (tagIndex != TagManager::INVALID_INDEX)
		unregisterTaggedEntity(e, tagIndex);
	else
		SN_WARNING("Scene::unregisterTaggedEntity: tag '" << tag << "' doesn't exists");
	return tagIndex;
}

//------------------------------------------------------------------------------
void Scene::unregisterTaggedEntity(Entity & e, u32 tagIndex)
{
	if (!m_tagManager.remove(tagIndex, &e))
	{
		std::string tagName = "<undefined>";
		m_tagManager.getTagName(tagIndex, tagName);
		SN_WARNING("Scene::unregisterTaggedEntity: entity " << e.toString() << " was not registered with tag " << tagName);
	}
}

//------------------------------------------------------------------------------
Entity * Scene::getTaggedEntity(const std::string & tag)
{
	u32 tagIndex = m_tagManager.getTagIndex(tag);
	if (tagIndex == TagManager::INVALID_INDEX)
		return nullptr;

	const std::unordered_set<Entity*> taggedEntities = m_tagManager.getObjectsByTag(tagIndex);
	if (taggedEntities.empty())
		return nullptr;

	return *taggedEntities.begin();
}

//------------------------------------------------------------------------------
std::vector<Entity*> Scene::getTaggedEntities(const std::string & tag)
{
	std::vector<Entity*> entities;

	u32 tagIndex = m_tagManager.getTagIndex(tag);
	if (tagIndex == TagManager::INVALID_INDEX)
		return entities;

	const std::unordered_set<Entity*> taggedEntities = m_tagManager.getObjectsByTag(tagIndex);
	if (taggedEntities.empty())
		return entities;	

	entities.reserve(taggedEntities.size());
    for (auto it = taggedEntities.begin(); it != taggedEntities.end(); ++it)
        entities.push_back(*it);

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
    m_updateManager.update();

	// TODO Destroy entities with flag DESTROY_LATE set
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

} // namespace sn

