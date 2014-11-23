#include <sstream>

#include <core/util/typecheck.hpp>
#include <core/util/Log.hpp>
#include <core/json/json_utils.hpp>

#include "AbstractTransform.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Scene.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Entity::Entity() :
    ASScriptObject(),
    m_id(0),
    r_parent(nullptr),
    r_actor(nullptr),
    m_layerIndex(0), // default layer
    r_scene(nullptr),
    m_tags(0)
{
#ifdef SN_BUILD_DEBUG
    setName("_entity_");
#endif

    m_flags.set(EF_ACTIVE);
}

//------------------------------------------------------------------------------
// Private
Entity::~Entity()
{
}

//------------------------------------------------------------------------------
void Entity::releaseFromScene()
{
    // Clear components
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        Component * component = (*it);
        component->onDestroy();
        delete component;
    }

    // Clear tags
    for (u8 tagIndex = 0; tagIndex < TagManager::MAX_TAGS; ++tagIndex)
    {
        if (hasTag(tagIndex))
        {
            r_scene->tagManager.onEntityUntagged(this, tagIndex);
        }
    }

#ifdef SN_BUILD_DEBUG
    if (release() != 0)
    {
        SN_WARNING("Entity released from scene, but still referenced (name: \"" << m_name << "\")");
    }
#else
    release();
#endif

    r_scene = nullptr;
}

//------------------------------------------------------------------------------
Scene & Entity::scene() const
{
    return *r_scene;
}

//------------------------------------------------------------------------------
void Entity::destroyLater()
{
    m_flags.set(EF_DESTROY_LATE, true);
    // Note: when an entity is marked as DESTROY_LATE,
    // its children will be destroyed too. See Scene.cpp for implementation details.
}

//------------------------------------------------------------------------------
bool Entity::active() const
{
    return m_flags[EF_ACTIVE];
}

//------------------------------------------------------------------------------
void Entity::setParent(Entity * newParent)
{
    // Don't do anything if the parent is the same
    if(newParent == r_parent)
    {
        return;
    }

    // If the entity has already a parent
    if(r_parent != nullptr)
    {
#ifdef SN_BUILD_DEBUG
        SN_DLOG("Unparenting [" << m_id << "]\"" << getName() << "\" from [" << r_parent->id() << "]\"" << r_parent->getName() << '"');
#endif
        // Remove the entity from its last parent
        for(auto it = r_parent->m_children.begin(); it != r_parent->m_children.end(); ++it)
        {
            if(*it == this)
            {
                r_parent->m_children.erase(it);
                break;
            }
        }
    }

    // Set new parent

    r_parent = newParent;

    // If the new parent is not null
    if(r_parent != nullptr)
    {
#ifdef SN_BUILD_DEBUG
        SN_DLOG("Parenting [" << m_id << "]\"" << getName() << "\" to [" << r_parent->id() << "]\"" << r_parent->getName() << '"');
#endif
        // Add the child to its parent
        r_parent->m_children.push_back(this);
    }

    AbstractTransform * transform = getComponent<AbstractTransform>();
    if (transform)
    {
        // Notify transform for parent change
        transform->onParentChanged();
    }
}

//------------------------------------------------------------------------------
void Entity::uparentChildren()
{
    // Make a copy of the child references to avoid concurrent modification
    // involved by setParent(). After this, m_children should be empty.
    auto childrenCopy = m_children;
    for(auto it = childrenCopy.begin(); it != childrenCopy.end(); ++it)
    {
        (*it)->setParent(nullptr);
    }
}

//------------------------------------------------------------------------------
Entity & Entity::child(u32 index)
{
#ifdef SN_BUILD_DEBUG
    if(index >= m_children.size())
    {
        SN_ERROR("Entity::child(): child index is out of bounds "
            "(index=" << index << ", "
            "size=" << m_children.size() << ", "
            "entity=[" << m_id << "]\"" << getName() << "\")");
    }
#endif
    assert(index < m_children.size());
    return *(m_children[index]);
}

//------------------------------------------------------------------------------
bool Entity::activeInHierarchy() const
{
    if(active())
    {
        if(r_parent)
        {
            return r_parent->activeInHierarchy();
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
void Entity::setActive(bool active)
{
    m_flags.set(EF_ACTIVE, active);
}

//------------------------------------------------------------------------------
void Entity::setSticky(bool isSticky)
{
    m_flags.set(EF_STICKY, isSticky);
}

//------------------------------------------------------------------------------
void Entity::setLayerByName(const std::string & layerName)
{
    const Layer * layerObj = r_scene->layers[layerName];
    m_layerIndex = layerObj->index;
}

//------------------------------------------------------------------------------
void Entity::setLayerByIndex(u32 layerIndex)
{
#ifdef SN_BUILD_DEBUG
    if(layerIndex >= LayerMap::COUNT)
    {
        SN_ERROR("Entity::setLayerByIndex: invalid index (" << layerIndex << ')');
        return;
    }
#endif
    m_layerIndex = layerIndex;
}

//------------------------------------------------------------------------------
const Layer & Entity::layer() const
{
    return r_scene->layers[m_layerIndex];
}

//------------------------------------------------------------------------------
void Entity::setActor(Component * actor)
{
#ifdef SN_BUILD_DEBUG
    if(r_actor != nullptr)
    {
        // Usually, an actor component stays the same for the whole lifetime of the entity,
        // as it masters the other componnets. This warning only notices
        // a potential mistake.
        SN_WARNING("Entity::setActor(): Actor property was already set");
        SN_MORE("On entity[" << m_id << "] '" << m_name << "'");
    }
#endif
    r_actor = actor;
}

//------------------------------------------------------------------------------
Component * Entity::getComponent(const ObjectType & cmpType, bool includeInheritance) const
{
    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        Component * cmp = (*it);
        if(cmp->objectType().is(cmpType, includeInheritance))
        {
            return cmp;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Component * Entity::addComponent(Component * newCmp)
{
    assert(newCmp != nullptr);

    m_components.push_back(newCmp);

    newCmp->onAdd(this);

    newCmp->onCreate();

    return newCmp;
}

//------------------------------------------------------------------------------
void Entity::removeComponent(Component * cmp)
{
    assert(cmp != nullptr);

    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        if((*it) == cmp)
        {
            cmp->onDestroy();
            m_components.erase(it);
            delete cmp;
            return;
        }
    }

    #ifdef SN_BUILD_DEBUG
    SN_WARNING("Entity::removeComponent: not found " << cmp->objectType().toString());
    #endif
}

//------------------------------------------------------------------------------
void Entity::sendMessage(const std::string & msg)
{
    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        (*it)->onMessage(msg);
    }
}

//------------------------------------------------------------------------------
void Entity::serialize(JsonBox::Value & o)
{
    // Meta

    o["id"]       = (s32)m_id;
    o["flags"]    = (u8)(m_flags.to_ulong());
    o["layer"]    = (s32)m_layerIndex; // TODO fix JsonBox so it accepts unsigned integers
    o["name"]     = getName();
    o["tags"]     = (s32)m_tags;

    // Components

    JsonBox::Array componentListData;
    componentListData.resize(m_components.size());
    u32 i = 0;
    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        JsonBox::Value & componentData = componentListData[i];
        Component::serialize(*it, componentData);
        ++i;
    }
    o["components"] = componentListData;
}

//------------------------------------------------------------------------------
void Entity::unserialize(JsonBox::Value & o)
{
    // Meta

    m_id = o["id"].getInt();
    m_flags = std::bitset<8>((u8)(o["flags"].getInt()));
    m_layerIndex = o["layer"].getInt();
    m_tags = o["tags"].getInt();
    setName(o["name"].getString());

    // Components

    JsonBox::Value & componentListData = o["components"];
    u32 n = componentListData.getArray().size();
    for(u32 i = 0; i < n; ++i)
    {
        Component * component = Component::unserialize(componentListData[i]);

        if(!checkComponentAddition(component->objectType(), "Entity::unserialize"))
        {
            delete component;
            continue;
        }

        addComponent(component);
    }
}

//------------------------------------------------------------------------------
void Entity::postUnserialize(JsonBox::Value & o)
{
    for(auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        (*it)->postUnserialize();
    }
}

//------------------------------------------------------------------------------
bool Entity::checkComponentAddition(const ObjectType & ct, const std::string & context)
{
    // Check component derivation
    if(!ct.is(sn::Component::sObjectType()))
    {
        SN_ERROR(context << ": Cannot add a non-component object (" << ct.toString() << ")");
        return false;
    }

    // Check component type ID
    if(ct.ID == 0)
    {
        SN_ERROR(context << ": "
            "The native component " << ct.name << " has no ID. "
            "It must be registered on application start.");
        SN_MORE("on entity \"" << m_name << '"');
        return false;
    }

    /*
    // Check duplicates
    auto componentIt = m_components.find(ct.ID);
    if(componentIt != m_components.end())
    {
        log.err() << context << ": "
            "cannot add two components of the same type ! " << log.endl();
        log.more() << ct.toString() << log.endl();
        log.more() << "on entity \"" << m_name << '"' << log.endl();

        return false;
    }

    // If the component is not a behaviour, test its unicity within the entity
    if(!ct.is(Behaviour::sObjectType()))
    {
        // Search for another component of the same group
        Component * duplicate = getComponent(ct);

        // If another has been found
        if(duplicate != nullptr)
        {
            log.err() << context << ": "
                "cannot add component, only one of its group is allowed !" << log.endl();
            log.more() << "adding: " << ct.toString() << log.endl();
            log.more() << "the entity already has: " << duplicate->objectType().toString() << log.endl();
            log.more() << "on entity \"" << m_name << '"' << log.endl();

            return false;
        }
    }
    */

    return true;
}

//------------------------------------------------------------------------------
void Entity::addTag(u8 tagIndex)
{
    m_tags |= 1 << tagIndex;
    r_scene->tagManager.onEntityTagged(this, tagIndex);
}

//------------------------------------------------------------------------------
void Entity::removeTag(u8 tagIndex)
{
    m_tags &= ~(1 << tagIndex);
    r_scene->tagManager.onEntityUntagged(this, tagIndex);
}

//------------------------------------------------------------------------------
bool Entity::hasTag(u8 tagIndex) const
{
    return (m_tags & (1 << tagIndex)) != 0;
}

//------------------------------------------------------------------------------
void Entity::addTag(const std::string & tagName)
{
    s8 i = r_scene->tagManager.indexFromName(tagName);
    SN_ASSERT(i >= 0, "addTag: TagManager returned invalid index! (" << i << ")");
    addTag(i);
}

//------------------------------------------------------------------------------
void Entity::removeTag(const std::string & tagName)
{
    s8 i = r_scene->tagManager.indexFromName(tagName);
    SN_ASSERT(i >= 0, "removeTag: TagManager returned invalid index! (" << i << ")");
    removeTag(i);
}

//------------------------------------------------------------------------------
bool Entity::hasTag(const std::string & tagName) const
{
    s8 i = r_scene->tagManager.indexFromName(tagName);
    if(i >= 0)
    {
        return hasTag(i);
    }
    else
    {
        return false;
    }
}

} // namespace zn





