/*
Entity.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/util/Log.hpp>
#include <core/util/assert.hpp>
#include <sstream>

#include "Entity.hpp"
#include "Scene.hpp"

#define SN_JSON_ENTITY_CHILDREN_TAG "_children"

namespace sn
{

//------------------------------------------------------------------------------
Entity::Entity() : ScriptObject(),
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

    // We need to iterate this way because removeTag() modifies m_tags
    while (!m_tags.empty())
        removeTag(*m_tags.begin());

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
void Entity::setUpdatable(bool updatable, s16 order, s16 layer)
{
	// TODO FIXME Set the flag!!
    if (getFlag(SN_EF_UPDATABLE) ^ updatable)
    {
        Scene * scene = getScene();
        if (scene)
        {
			setFlag(SN_EF_UPDATABLE, updatable);
            if (updatable)
                scene->registerUpdatableEntity(*this, order, layer);
            else
                scene->unregisterUpdatableEntity(*this);
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
    if (m_tags.insert(tag).second)
    {
        Scene * scene = getScene();
        if (scene)
            scene->registerTaggedEntity(*this, tag);
        else
            SN_ERROR("Entity::addTag: scene not found from entity " << toString());
    }
    else
    {
        SN_WARNING("Entity::addTag: tag " << tag << " already set on entity " << toString());
    }
}

//------------------------------------------------------------------------------
void Entity::removeTag(const std::string tag)
{
    if (m_tags.erase(tag))
    {
        Scene * scene = getScene();
        if (scene)
            scene->unregisterTaggedEntity(*this, tag);
        else
            SN_ERROR("Entity::removeTag: scene not found from entity " << toString());
    }
    else
    {
        SN_WARNING("Entity::addTag: tag " << tag << " already set on entity " << toString());
    }
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
void Entity::setParent(Entity * newParent)
{
    // TODO FIXME Scene registrations such as update or tags must be notified too if the scene changes!

    if (r_parent != nullptr && newParent == nullptr)
    {
        r_parent->removeChild(this);
        r_parent = newParent;
    }
    else if (newParent != nullptr)
    {
        if (r_parent == nullptr)
        {
            r_parent = newParent;
            r_parent->addChild(this);

            if (newParent->isInstanceOf<Scene>() && r_scene == nullptr)
            {
                r_scene = static_cast<Scene*>(newParent);

                // TODO have a setScene() function performing all registration stuff
                for (auto it = m_tags.begin(); it != m_tags.end(); ++it)
                {
                    r_scene->registerTaggedEntity(*this, *it);
                }

                propagateOnReady();
            }
        }
        else
        {
            // Just swap ownership
            r_parent->removeChild(this);
            r_parent = newParent;
            r_parent->addChild(this);
        }
    }
}

//------------------------------------------------------------------------------
void Entity::propagateOnReady()
{
    onReady();
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
        (*it)->onReady();
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
// Static
void Entity::serialize(JsonBox::Value & o, Entity & e, const SerializationContext & context)
{
    o[SN_JSON_TYPE_TAG] = Entity::__sGetClassName();
    e.serializeState(o, context);
    if (e.getChildCount() != 0)
    {
        JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
        for (u32 i = 0; i < e.getChildCount(); ++i)
        {
            Entity * child = e.getChildByIndex(i);
            serialize(a[i], *child, context);
        }
    }
}

//------------------------------------------------------------------------------
// Static
Entity * Entity::unserialize(JsonBox::Value & o, Entity * parent, const SerializationContext & context)
{
    std::string typeName = o[SN_JSON_TYPE_TAG].getString();
    ObjectType * ot = ObjectTypeDatabase::get().getType(typeName);
    if (ot)
    {
        Object * obj = instantiateDerivedObject(typeName, Entity::__sGetClassName());
        if (obj)
        {
            Entity * e((Entity*)obj);
            e->unserializeState(o, context);
            if (o[SN_JSON_ENTITY_CHILDREN_TAG].isArray())
            {
                JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
                u32 len = a.getArray().size();
                for (u32 i = 0; i < len; ++i)
                {
                    Entity::unserialize(a[i], e, context);
                }
            }
            e->setParent(parent);
            return e;
        }
        // Error message already handled by the instantiate helper
    }
    else
    {
        SN_ERROR("Unknown object object type from JSON (name=" << typeName << ")");
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void Entity::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    o["name"] = m_name;
    o["enabled"] = isEnabledSelf();
    sn::serialize(o["tags"], m_tags);

    // TODO Serialize script

}

//------------------------------------------------------------------------------
void Entity::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    m_name = o["name"].getString();
    
    bool bEnabled = false;
    sn::unserialize(o["enabled"], bEnabled, true);
    setFlag(SN_EF_ENABLED, bEnabled);

    sn::unserialize(o["tags"], m_tags);

    auto & scripts = o["scripts"];
    if (scripts.isArray())
    {
        std::string classPath = scripts[(size_t)0].getString();
        if (!classPath.empty())
        {
            m_script.create(classPath);
        }
    }

    // TODO Unserialize script

}

} // namespace sn

