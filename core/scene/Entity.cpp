#include <core/util/Log.hpp>
#include <core/util/assert.hpp>
#include <sstream>

#include "Entity.hpp"
#include "Scene.hpp"

#define SN_JSON_ENTITY_CHILDREN_TAG "_children"

namespace sn
{

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
    if (getFlag(SN_EF_UPDATABLE) ^ updatable)
    {
        SceneRef scene = getScene();
        if (scene)
        {
            if (updatable)
                scene->registerUpdatableEntity(shared_from_this(), order, layer);
            else
                scene->unregisterUpdatableEntity(shared_from_this());
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
void Entity::addTag(const std::string & tag)
{
    if (m_tags.insert(tag).second)
    {
        SceneRef scene = getScene();
        if (scene)
            scene->registerTaggedEntity(shared_from_this(), tag);
        else
            SN_ERROR("Entity::addTag: scene not found from entity " << toString());
    }
    else
    {
        SN_WARNING("Entity::addTag: tag " << tag << " already set on entity " << toString());
    }
}

//------------------------------------------------------------------------------
void Entity::removeTag(const std::string & tag)
{
    if (m_tags.erase(tag))
    {
        SceneRef scene = getScene();
        if (scene)
            scene->unregisterTaggedEntity(shared_from_this(), tag);
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
        ss << "[unnamed_entity]";
    else
        ss << m_name;
    return ss.str();
}

//------------------------------------------------------------------------------
void Entity::setParent(Entity::Ref newParent)
{
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
            r_parent->addChild(shared_from_this());

            if (newParent->isInstanceOf<Scene>() && r_scene.use_count() == 0)
            {
                r_scene = std::static_pointer_cast<Scene>(newParent);
                propagateOnReady();
            }
        }
        else
        {
            // Just swap ownership
            r_parent->removeChild(this);
            r_parent = newParent;
            r_parent->addChild(shared_from_this());
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
Entity::Ref Entity::getRoot() const
{
    if (r_parent)
    {
        Entity::Ref parent = r_parent;
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
SceneRef Entity::getScene() const
{
    SceneRef scene = r_scene.lock();
    if (!scene)
    {
        Entity::Ref root = getRoot();
        if (root && root->isInstanceOf<Scene>())
        {
            scene = std::static_pointer_cast<Scene>(root);
            r_scene = scene;
        }
    }
    return scene;
}

//------------------------------------------------------------------------------
Entity::Ref Entity::getChildByName(const std::string & name) const
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->getName() == name)
            return *it;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Entity::Ref Entity::getChildByType(const std::string & name) const
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
Entity::Ref Entity::addChild(Entity::Ref child)
{
#ifdef SN_BUILD_DEBUG
    if (child->getParent().get() != this)
    {
        SN_WARNING("Entity::addChild: parent is not matching with " << toString());
    }
    if (containsChild(child))
    {
        SN_ERROR("Entity::addChild: already contained in entity " << toString());
        return nullptr;
    }
#endif
    m_children.push_back(child);
    return child;
}

//------------------------------------------------------------------------------
u32 Entity::indexOfChild(const Entity::Ref child) const
{
    u32 i = 0;
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it) == child)
            break;
        ++i;
    }
    return false;
}

//------------------------------------------------------------------------------
Entity::Ref Entity::createChild(const std::string & typeName)
{
    Entity::Ref child = nullptr;

    if (typeName.empty())
    {
        child.reset(new Entity());
    }
    else
    {
        Object * obj = instantiateDerivedObject(typeName, Entity::__sGetBaseClassName());
        if (obj)
            child.reset((Entity*)obj);
    }

    if (child)
    {
        // Note: at this point, the entity MUST be owned by at least one shared_ptr,
        // because setParent internally sets a weak_ptr, and after this call,
        // the shared_from_this() will be released.
        child->setParent(shared_from_this());
    }

    return child;
}

//------------------------------------------------------------------------------
Entity::Ref Entity::requireChild(const std::string & typeName)
{
    Entity::Ref e = getChildByType(typeName);
    if (e == nullptr)
        e = createChild(typeName);
    return e;
}

//------------------------------------------------------------------------------
u32 Entity::removeChild(Entity::Ref child)
{
    u32 i = indexOfChild(child);
    removeChild(i);
    return i;
}

//------------------------------------------------------------------------------
u32 Entity::removeChild(Entity * child)
{
    u32 i = 0;
    for (; i < m_children.size(); ++i)
    {
        if (m_children[i].get() == child)
        {
            removeChild(i);
            break;
        }
    }
    return i;
}

//------------------------------------------------------------------------------
void Entity::removeChild(u32 index)
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
void Entity::destroy()
{
    onDestroy();
    setFlag(SN_EF_DESTROYED, true);
    // TODO Instant destruction
}

//------------------------------------------------------------------------------
void Entity::destroyLater()
{
    setFlag(SN_EF_DESTROYED, true);
}

//------------------------------------------------------------------------------
// Static
void Entity::serialize(JsonBox::Value & o, Entity::Ref e)
{
    SN_ASSERT(e.get() != nullptr, "Cannot serialize null entity");

    o[SN_JSON_TYPE_TAG] = Entity::__sGetClassName();
    e->serializeState(o);
    if (e->getChildCount() != 0)
    {
        JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
        for (u32 i = 0; i < e->getChildCount(); ++i)
        {
            Entity::Ref child = e->getChildByIndex(i);
            serialize(a[i], child);
        }
    }
}

//------------------------------------------------------------------------------
// Static
Entity::Ref Entity::unserialize(JsonBox::Value & o, Entity::Ref parent)
{
    std::string typeName = o[SN_JSON_TYPE_TAG].getString();
    ObjectType * ot = ObjectTypeDatabase::get().getType(typeName);
    if (ot)
    {
        Object * obj = instantiateDerivedObject(typeName, Entity::__sGetClassName());
        if (obj)
        {
            Entity::Ref e((Entity*)obj);
            e->unserializeState(o);
            if (o[SN_JSON_ENTITY_CHILDREN_TAG].isArray())
            {
                JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
                u32 len = a.getArray().size();
                for (u32 i = 0; i < len; ++i)
                {
                    Entity::unserialize(a[i], e);
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
void Entity::serializeState(JsonBox::Value & o)
{
    o["name"] = m_name;
    o["enabled"] = isEnabledSelf();
    sn::serialize(o, m_tags);
}

//------------------------------------------------------------------------------
void Entity::unserializeState(JsonBox::Value & o)
{
    m_name = o["name"].getString();
    setEnabled(o["enabled"].getBoolean());
    sn::serialize(o, m_tags);

    // TODO FIXME Do this only when the scene changes to non-null
    SceneRef scene = getScene();
    for (auto it = m_tags.begin(); it != m_tags.end(); ++it)
    {
        scene->registerTaggedEntity(shared_from_this(), *it);
    }
}

} // namespace sn

