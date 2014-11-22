/*
Scene.cpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the Plane Framework project.
*/

#include <core/util/Log.hpp>

#include "Scene.hpp"
#include "Transform.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Scene::Scene() :
    automaticTransforms(true),
    m_nextID(0)
{
    layers.setLayer(0, "default");
}

//------------------------------------------------------------------------------
Scene::~Scene()
{
    clear();
}

//------------------------------------------------------------------------------
Entity * Scene::createEntity(std::string name, sn::Vector3f pos)
{
    Entity * e = new Entity();

    if(automaticTransforms)
    {
        e->addComponent<Transform>();
    }

    e->m_id = m_nextID++;

    AbstractTransform * transform = e->getComponent<AbstractTransform>();
    if (transform)
    {
        transform->setPosition(pos);
    }

    if(name.empty())
    {
        std::stringstream ss;
        ss << "entity" << e->id();
        name = ss.str();
    }
    e->setName(name);

    e->setScene(this);

    m_entities.push_back(e);

#ifdef SN_BUILD_DEBUG
    SN_DLOG("Scene::addEntity: [" << e->id() << "] " << name << " at (" << pos.x << ", " << pos.y << ")");
#endif

    return e;
}

//------------------------------------------------------------------------------
Entity * Scene::findEntityFromID(EntityID id) const
{
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
    {
        if((*it)->id() == id)
            return *it;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Entity * Scene::findEntityFromName(const std::string & name)
{
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
    {
        if((*it)->name() == name)
            return *it;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void Scene::registerBehaviour(Behaviour * behaviour)
{
    #ifdef ZN_DEBUG
    assert(behaviour != nullptr);
    assert(behaviour->objectType().is(Behaviour::sObjectType()));
    #endif

    s32 updateOrder = behaviour->getUpdateOrder();
    m_behaviours[updateOrder].add(behaviour);
}

//------------------------------------------------------------------------------
void Scene::unregisterBehaviour(Behaviour * behaviour)
{
    s32 updateOrder = behaviour->getUpdateOrder();
    m_behaviours[updateOrder].remove(behaviour);
}

//------------------------------------------------------------------------------
// Deletes the given entity if its DESTROY_LATE flag is true,
// then returns true if that was the case, false otherwise
bool isLateDestroyThenDelete(Entity * e)
{
    if(e->flag(EF_DESTROY_LATE))
    {
#ifdef SN_BUILD_DEBUG
        SN_DLOG("just before destroy entity \"" << e->name() << '"');
#endif

        // Detach from its parent
        e->setParent(nullptr);
        // Detach its children
        e->uparentChildren();

        // Then delete it
        delete e;

        return true;
    }
    else
        return false;
}

//------------------------------------------------------------------------------
// Sets the DESTROY_LATE flag on the given entity and all of its children
void propagateDestroyLate(Entity & e)
{
    for(u32 i = 0; i < e.childCount(); ++i)
    {
        Entity & childEntity = e.child(i);
        childEntity.destroyLater();
        propagateDestroyLate(childEntity);
    }
}

//------------------------------------------------------------------------------
void Scene::update(sn::Time deltaTime)
{
    m_deltaTime = deltaTime;

    // Update behaviours in their update order
    for(auto it = m_behaviours.begin(); it != m_behaviours.end(); ++it)
    {
        it->second.update();
    }

    // Mark all entities that should be destroyed :
    // If an entity has children, mark them too
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
    {
        Entity & e = **it;
        if(e.m_flags[EF_DESTROY_LATE])
        {
            propagateDestroyLate(e);
        }
    }

    // Destroy entities with DESTROY_LATE flag set
    m_entities.remove_if(isLateDestroyThenDelete);

    // Update physics
    //physics.update();

    // Update animations
    //animators.update();

    // Update sound
    //audioSystem.update(deltaTime);

    // Update render system
    //renderSystem.update();
}

//------------------------------------------------------------------------------
void Scene::clear()
{
    for(Entity *& e : m_entities)
    {
        // Note: the destruction of an entity triggers the destruction of its
        // components too
        delete e;
    }
    m_entities.clear();

    m_nextID = 0;
}

//------------------------------------------------------------------------------
// Deletes the given entity if its CROSS_SCENE flag is false.
// Returns true if the entity has been deleted.
bool isNotSceneCrossThenDelete(Entity * e)
{
    if(!e->flag(EF_CROSS_SCENE))
    {
        delete e;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
void Scene::clearAllButCrossEntities()
{
    m_entities.remove_if(isNotSceneCrossThenDelete);
}

//------------------------------------------------------------------------------
//void Scene::makeCurrent()
//{
//	g_currentSceneRef = this;
//}

//------------------------------------------------------------------------------
//Scene & Scene::current()
//{
//	return *g_currentSceneRef;
//}

//------------------------------------------------------------------------------
//void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	renderSystem.draw(target, states);
//}

//------------------------------------------------------------------------------
//void Scene::onScreenResized(sf::Vector2u resolution)
//{
//	renderSystem.onScreenResized(resolution);
//}

//------------------------------------------------------------------------------
void Scene::serialize(JsonBox::Value & o)
{
    JsonBox::Array entityListData;
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
    {
        JsonBox::Value entityData;
        (*it)->serialize(entityData);
        entityListData.push_back(entityData);
    }

    o["nextID"] = (s32)m_nextID;
    o["entities"] = entityListData;
    o["automaticTransforms"] = automaticTransforms;
}

//------------------------------------------------------------------------------
void Scene::unserialize(JsonBox::Value & o)
{
    // Unserialized content will replace old content
    clear();

    u32 computedNextID = 0;

    std::unordered_map<u32,Entity*> id2entity;
    std::list<std::pair<Entity*,JsonBox::Value*>> entityDataList;

    JsonBox::Value & entityListData = o["entities"];
    u32 n = entityListData.getArray().size();
    for(u32 i = 0; i < n; ++i)
    {
        JsonBox::Value & entityData = entityListData[i];

        Entity * entity = createEntity();
        u32 id = entityData["id"].getInt();

        auto it = id2entity.find(id);
        if(it != id2entity.end())
        {
            SN_ERROR("Conflicting entity found on scene loading : "
                "\"" << entityData["name"] << "\"[" << id << "] "
                "conflicts with "
                "\"" << it->second->name() << "\"[" << it->second->id() << "]");
            continue;
        }
        else
        {
            id2entity.insert(std::make_pair(id, entity));
        }

        entity->unserialize(entityListData[i]);

        entityDataList.push_back(std::make_pair(entity, &entityData));

        if(computedNextID <= id)
        {
            computedNextID = id+1;
        }
    }

    m_nextID = computedNextID;
    automaticTransforms = o["automaticTransforms"].getBoolean();

    // Post-unserialize

    for(auto it = entityDataList.begin(); it != entityDataList.end(); ++it)
    {
        Entity * entity = it->first;
        JsonBox::Value * entityData = it->second;

        entity->postUnserialize(*entityData);
    }
}

//------------------------------------------------------------------------------
bool Scene::saveToFile(const std::string & filePath)
{
#ifdef SN_BUILD_DEBUG
    SN_DLOG("Saving scene as \"" << filePath << '"');
    sn::Clock timer;
#endif

    std::ofstream ofs(filePath.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);
    if(ofs.good())
    {
        JsonBox::Value v;
        serialize(v);
        v.writeToStream(ofs);
        ofs.close();
    }
    else
    {
        SN_ERROR("Failed to load scene from \"" << filePath << '"');
        return false;
    }

#ifdef SN_BUILD_DEBUG
    f32 timeSpent = timer.getElapsedTime().asSeconds();
    SN_DLOG("Took " << timeSpent << "s to serialize the scene as a file.");
#endif
    return true;
}

//------------------------------------------------------------------------------
bool Scene::loadFromFile(const std::string & filePath)
{
#ifdef SN_BUILD_DEBUG
    SN_DLOG("Loading scene from \"" << filePath << '"');
    sn::Clock timer;
#endif

    std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
    if(ifs.good())
    {
        JsonBox::Value v;
        unserialize(v);
        v.loadFromStream(ifs);
        ifs.close();
    }
    else
    {
        SN_ERROR("Failed to save scene to \"" << filePath << '"');
        return false;
    }

#ifdef SN_BUILD_DEBUG
    f32 timeSpent = timer.getElapsedTime().asSeconds();
    SN_DLOG("Took " << timeSpent << "s to serialize the scene as a file.");
#endif
    return true;
}

} // namespace sn






