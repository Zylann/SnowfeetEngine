/*
Entity.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/util/Log.h>
#include <core/util/assert.h>
#include <sstream>

#include "Entity.h"
#include "Scene.h"
#include "../app/Application.h"

#define SN_JSON_ENTITY_CHILDREN_TAG "_children"

namespace sn
{

//------------------------------------------------------------------------------
Entity::Entity() : ScriptableObject(),
    m_flags(1 << SN_EF_ENABLED),
    r_parent(nullptr),
    r_scene(nullptr)
{
}

//------------------------------------------------------------------------------
Entity::~Entity()
{
    if (r_parent)
        r_parent->removeChild(this);
    destroyChildren();

    removeAllTags();

	// Unregister update callback
    if (getFlag(SN_EF_UPDATABLE))
    	setUpdatable(false);
    if (getFlag(SN_EF_SYSTEM_EVENT_LISTENER))
        listenToSystemEvents(false);

    //SN_LOG("Entity " << getName() << " destroyed");
}

//------------------------------------------------------------------------------
void Entity::setName(const std::string & name)
{
    m_name = name;
}

//------------------------------------------------------------------------------
void Entity::setFlag(u32 position, bool v)
{
    m_flags.set(position, v);
}

//------------------------------------------------------------------------------
bool Entity::isEnabled() const
{
    if (m_flags.at(SN_EF_ENABLED))
        return r_parent ? r_parent->isEnabled() : true;
    else
        return false;
}

//------------------------------------------------------------------------------
void Entity::setEnabled(bool e)
{
    m_flags.set(SN_EF_ENABLED, e);
}

//------------------------------------------------------------------------------
void Entity::setUpdatable(bool updatable, const std::string layerName)
{
	// TODO FIXME Set the flag!!
    if (getFlag(SN_EF_UPDATABLE) ^ updatable)
    {
        Scene * scene = getScene();
        if (scene)
        {
			setFlag(SN_EF_UPDATABLE, updatable);
            UpdateManager & manager = scene->getUpdateManager();
            if (updatable)
            {
                manager.addEntity(*this, layerName.empty() ? UpdateManager::DEFAULT_LAYER : layerName);
            }
            else
            {
                manager.removeEntity(*this, layerName);
            }
        }
        else
        {
            SN_ERROR("Entity::setUpdatable: scene not found from entity " << toString());
        }
    }
    else
    {
        SN_WARNING("Entity::setUpdatable: no change made (entity " << toString() << ")");
    }
}

//------------------------------------------------------------------------------
void Entity::listenToSystemEvents(bool enable)
{
	// TODO FIXME Set the flag!!
	if (getFlag(SN_EF_SYSTEM_EVENT_LISTENER) ^ enable)
	{
		Scene * scene = getScene();
		if (scene)
		{
			setFlag(SN_EF_SYSTEM_EVENT_LISTENER, enable);
			if (enable)
				scene->registerEventListener(*this);
			else
				scene->unregisterEventListener(*this);
		}
        else
        {
            SN_ERROR("Entity::listenToSystemEvents: scene not found from entity " << toString());
        }
	}
    else
    {
        SN_WARNING("Entity::listenToSystemEvents: no change made (entity " << toString() << ")");
    }
}

//------------------------------------------------------------------------------
void Entity::addTag(const std::string & tag)
{
	Scene * scene = getScene();
	if (scene)
	{
		u32 tagIndex = scene->registerTaggedEntity(*this, tag);
		if (tagIndex != Scene::TagManager::INVALID_INDEX)
			m_tags.set(tagIndex, true);
	}
	else
	{
		SN_ERROR("Entity::addTag: scene not found from entity " << toString());
	}
}

//------------------------------------------------------------------------------
void Entity::removeTag(const std::string tag)
{
	Scene * scene = getScene();
	if (scene)
	{
        u32 tagIndex = scene->unregisterTaggedEntity(*this, tag);
		if (tagIndex != Scene::TagManager::INVALID_INDEX)
			m_tags.set(tagIndex, false);
	}
	else
	{
		SN_ERROR("Entity::removeTag: scene not found from entity " << toString());
	}
}

//------------------------------------------------------------------------------
void Entity::removeAllTags()
{
	Scene * scene = getScene();
	if (scene)
	{
		for (u32 i = 0; i < MAX_TAGS; ++i)
		{
			if (m_tags.test(i))
			{
				scene->unregisterTaggedEntity(*this, i);
				m_tags.set(i, false);
			}
		}
	}
	else
	{
		if (m_tags.count() != 0)
			SN_ERROR("Entity::removeAllTags: scene not found from entity " << toString());
	}
}

//------------------------------------------------------------------------------
inline bool Entity::hasTag(const std::string & tag) const
{
	Scene * scene = getScene();
	if (scene)
	{
		u32 tagIndex = scene->getTagManager().getTagIndex(tag);
		if (tagIndex == Scene::TagManager::INVALID_INDEX)
			return false;
		return m_tags.test(tagIndex);
	}
	SN_ERROR("Coudln't query entity tag, the scene is not found");
	return false;
}

//------------------------------------------------------------------------------
std::string Entity::toString() const
{
    std::stringstream ss;
    if (m_name.empty())
        ss << "[unnamed_entity " << getObjectType().getName() << "]";
    else
        ss << m_name;
    return ss.str();
}

//------------------------------------------------------------------------------
void Entity::getTags(std::vector<std::string> & tags)
{
	Scene * scene = getScene();
	if (scene)
	{
		const Scene::TagManager & tagManager = scene->getTagManager();
		for (u32 i = 0; i < m_tags.size(); ++i)
		{
			std::string tagName;
			if (tagManager.getTagName(i, tagName))
				tags.push_back(tagName);
		}
	}
}

//------------------------------------------------------------------------------
void Entity::setParent(Entity * newParent)
{
    Scene * oldScene = getScene();

    // Update parent
    if (r_parent != nullptr && newParent == nullptr)
    {
        // Unparented
        r_parent->removeChild(this);
        r_parent = newParent;
    }
    else if (newParent != nullptr)
    {
        if (r_parent == nullptr)
        {
            // Parented for the first time
            r_parent = newParent;
            r_parent->addChild(this);
        }
        else
        {
            // Parent changed

            // Just swap ownership
            r_parent->removeChild(this);
            r_parent = newParent;
            r_parent->addChild(this);
        }
    }

    // Update scene
    r_scene = getScene();
    if (oldScene != r_scene)
    {
        onSceneChanged(oldScene, r_scene);
    }
}

//------------------------------------------------------------------------------
void Entity::onSceneChanged(Scene * oldScene, Scene * newScene)
{
    if (newScene == oldScene)
        return;

	std::vector<std::string> tagList;

    // Register to the new scene
    if (newScene)
    {
        // Update subscription
        if (getFlag(SN_EF_UPDATABLE))
        {
            std::string updateLayer;
            if (oldScene)
                oldScene->getUpdateManager().getEntityLayer(*this, updateLayer);
            newScene->getUpdateManager().addEntity(*this, updateLayer);
        }

        // System events subscription
        if (getFlag(SN_EF_SYSTEM_EVENT_LISTENER))
            newScene->registerEventListener(*this);

        // Tags registering
		if (oldScene)
		{
			// Get tags by name
			getTags(tagList);

			// Register them
			for (auto it = tagList.begin(); it != tagList.end(); ++it)
			{
				const std::string & tagName = *it;
				u32 tagIndex = newScene->registerTaggedEntity(*this, tagName);
				if (tagIndex != Scene::TagManager::INVALID_INDEX)
					m_tags.set(tagIndex, true);
			}
		}
    }
	else
	{
		SN_ERROR("No new scene, tag information will be lost!");
		m_tags.reset();
	}

    // Unregister from the old scene
    if (oldScene)
    {
        if (getFlag(SN_EF_UPDATABLE))
            oldScene->getUpdateManager().removeEntity(*this);

        if (getFlag(SN_EF_SYSTEM_EVENT_LISTENER))
            oldScene->unregisterEventListener(*this);

        for (auto it = tagList.begin(); it != tagList.end(); ++it)
        {
            oldScene->unregisterTaggedEntity(*this, *it);
        }
    }

    r_scene = newScene;

    // Notify children
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        Entity * child = *it;
        child->onSceneChanged(oldScene, newScene);
    }
}

//------------------------------------------------------------------------------
void Entity::onReady()
{
    if (!m_script.isNull())
    {
        m_script.callMethod("onReady");
    }
}

//------------------------------------------------------------------------------
void Entity::onFirstUpdate()
{
    if (!m_script.isNull())
    {
        m_script.callMethod("onFirstUpdate");
    }
}

//------------------------------------------------------------------------------
void Entity::onDestroy()
{
    if (!m_script.isNull())
    {
        m_script.callMethod("onDestroy");
    }
}

//------------------------------------------------------------------------------
void Entity::onUpdate()
{
    if (!m_script.isNull())
    {
        m_script.callMethod("onUpdate");
    }
}

//------------------------------------------------------------------------------
void Entity::propagateOnReady()
{
    onReady();
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->propagateOnReady();
}

//------------------------------------------------------------------------------
Entity * Entity::getRoot() const
{
    if (r_parent)
    {
        Entity * parent = r_parent;
        while (parent->getParent())
            parent = parent->getParent();
        return parent;
    }
    else
    {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
Scene * Entity::getScene() const
{
    Scene * scene = r_scene;
    if (!scene)
    {
        Entity * root = getRoot();
        if (root && root->isInstanceOf<Scene>())
        {
            scene = static_cast<Scene*>(root);
            r_scene = scene;
        }
    }
    return scene;
}

//------------------------------------------------------------------------------
Entity * Entity::getChildByName(const std::string & name) const
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->getName() == name)
            return *it;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Entity * Entity::getChildByType(const std::string & name) const
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->getObjectType().is(name))
            return *it;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
//Entity::Ref Entity::getChildByPointer(Entity * child)
//{
//    for (auto it = m_children.begin(); it != m_children.end(); ++it)
//    {
//        if (it->get() == child)
//            return *it;
//    }
//    return nullptr;
//}

//------------------------------------------------------------------------------
Entity * Entity::addChild(Entity * child)
{
#ifdef SN_BUILD_DEBUG
    if (child->getParent() != this)
    {
        SN_WARNING("Entity::addChild: parent is not matching with " << toString());
    }
    if (containsChild(child))
    {
        SN_ERROR("Entity::addChild: " << child->toString() << " already contained in entity " << toString());
        return nullptr;
    }
#endif
    m_children.push_back(child);
    //if (childIndex == -1)
    //    m_children.push_back(child);
    //else
    //{
    //    if (childIndex >= m_children.size())
    //        m_children.resize(childIndex + 1, nullptr);
    //    m_children[childIndex] = child;
    //}
    return child;
}

//------------------------------------------------------------------------------
u32 Entity::indexOfChild(const Entity * child) const
{
    u32 i = 0;
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it) == child)
            break;
        ++i;
    }
    return i;
}

//------------------------------------------------------------------------------
Entity * Entity::createChild(const std::string & typeName)
{
    Entity * child = nullptr;

    if (typeName.empty())
    {
        child = new Entity();
    }
    else
    {
        Object * obj = instantiateDerivedObject(typeName, Entity::__sGetBaseClassName());
        if (obj)
            child = (Entity*)obj;
    }

    if (child)
    {
        child->setParent(this);
    }

    return child;
}

//------------------------------------------------------------------------------
Entity * Entity::requireChild(const std::string & typeName)
{
    Entity * e = getChildByType(typeName);
    if (e == nullptr)
        e = createChild(typeName);
    return e;
}

//------------------------------------------------------------------------------
u32 Entity::removeChild(Entity * child)
{
    u32 i = indexOfChild(child);
    if (i != m_children.size())
        removeChildAtIndex(i);
    return i;
}

//------------------------------------------------------------------------------
void Entity::removeChildAtIndex(u32 index)
{
    if (index < m_children.size())
    {
        m_children[index] = nullptr;
        m_children[index] = m_children.back();
        m_children.pop_back();
    }
}

//------------------------------------------------------------------------------
void Entity::destroyChildren()
{
    auto children = m_children;
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        (*it)->destroy();
    }
    m_children.clear();
}

//------------------------------------------------------------------------------
void Entity::destroyChildren(std::function<bool(const Entity&)> f_mustBeDestroyed)
{
    auto children = m_children;
    for (u32 i = 0; i < children.size(); ++i)
    {
        Entity * child = children[i];
        if (f_mustBeDestroyed(*child))
        {
            child->destroy();

            // Swap old child position with the last one
            children[i] = children.back();

            // Pop back (faster than calling erase(), as it would involve a lot of shifting)
            children.pop_back();

            --i; // Note: can underflow by one, but the for loop will ++i anyway
        }
    }
    m_children = children;
}

//------------------------------------------------------------------------------
void Entity::destroy()
{
    if (!getFlag(SN_EF_DESTROYED))
    {
        onDestroy();
        setFlag(SN_EF_DESTROYED, true);
        releaseScript();
        // TODO Instant destruction
        release();
    }
    else
    {
        SN_ERROR("Attempt to call destroy() on an already destroyed entity!");
    }
}

//------------------------------------------------------------------------------
void Entity::destroyLater()
{
    SN_WARNING("Entity::destroyLater(): not implemented yet");
    // TODO Different flag?
    //setFlag(SN_EF_DESTROYED, true);
}

//------------------------------------------------------------------------------
void Entity::releaseScript()
{
    //m_script.setMemberNull("entity");
    m_script.releaseObject();
}

//------------------------------------------------------------------------------
void Entity::serializeState(Variant & o, const SerializationContext & context)
{
    o["name"] = m_name;
    o["enabled"] = isEnabledSelf();

	std::vector<std::string> tags;
	getTags(tags);
    sn::serialize(o["tags"], tags);

    // TODO Serialize script

}

//------------------------------------------------------------------------------
void Entity::unserializeState(const Variant & o, const SerializationContext & context)
{
    // Name
    m_name = o["name"].getString();
    
    // Enabled flag
    bool bEnabled = false;
    sn::unserialize(o["enabled"], bEnabled, true);
    setFlag(SN_EF_ENABLED, bEnabled);

    // Deserialize tags
    removeAllTags();
    std::unordered_set<std::string> tags;
    sn::unserialize(o["tags"], tags);
    for (auto it = tags.begin(); it != tags.end(); ++it)
    {
        const std::string & tagName = *it;
        addTag(tagName);
    }

    // Script
    auto & script = o["script"];
    if (script.isDictionary())
    {
        std::string classPath = script["class"].getString();
        if (!classPath.empty())
        {
            // TODO should be context.squirrelVM
            HSQUIRRELVM vm = Application::get().getScriptManager().getVM();

            if (m_script.create(vm, classPath))
            {
                // Set the "entity" member
                if (pushScriptObject(vm))
                {
                    HSQOBJECT entityObj;
                    sq_getstackobj(vm, -1, &entityObj);
                    m_script.setMember("entity", entityObj);
                    sq_pop(vm, 1); // pop entityObj
                }

				// Call onCreate
				m_script.callMethod("onCreate");
            }
        }
    }

    // TODO Unserialize script members

}

} // namespace sn

