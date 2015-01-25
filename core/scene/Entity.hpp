#ifndef __HEADER_SN_ENTITY__
#define __HEADER_SN_ENTITY__

#include <core/types.hpp>
#include <core/reflect/Object.hpp>
#include <core/util/RefCounted.hpp>
#include <core/json/json_utils.hpp>

#include <vector>
#include <string>
#include <bitset>
#include <unordered_set>

namespace sn
{

enum EntityFlags
{
    SN_EF_ENABLED = 0,
    SN_EF_DESTROYED = 1,
    SN_EF_FIRST_UPDATE = 2,
    SN_EF_UPDATABLE = 3
};

class Scene;
// Note: we can't use Scene::Ref here because it's impossible to forward-declare it
typedef std::shared_ptr<Scene> SceneRef;
typedef std::weak_ptr<Scene> SceneWeakRef;

class SN_API Entity : 
    public std::enable_shared_from_this<Entity>,
    public Object
{
public:

    SN_OBJECT(sn::Entity, sn::Object);

    Entity() :
        Object()
    {}

    virtual ~Entity()
    {
        if (r_parent)
            r_parent->removeChild(this);
        destroyChildren();
        for (auto it = m_tags.begin(); it != m_tags.end(); ++it)
            removeTag(*it);
    }

    inline const std::string & getName() const { return m_name; }
    void setName(const std::string & name);

    inline bool getFlag(u32 position) const { return m_flags.at(position); }
    void setFlag(u32 position, bool v);

    bool isEnabled() const;
    inline bool isEnabledSelf() const { return m_flags.at(SN_EF_ENABLED); }
    void setEnabled(bool e);

    void setUpdatable(bool enable=true, s16 order=0, s16 layer=0);

    inline bool hasTag(const std::string & tag) const { return m_tags.find(tag) != m_tags.end(); }
    void addTag(const std::string & tag);
    void removeTag(const std::string & tag);

    std::string toString() const;

    inline Entity::Ref getParent() const { return r_parent; }
    virtual void setParent(Entity::Ref newParent);
    Entity::Ref getRoot() const;

    SceneRef getScene() const;

    inline u32 getChildCount() const { return m_children.size(); }
    inline Entity::Ref getChildByIndex(u32 i) const { return m_children[i]; }
    Entity::Ref getChildByName(const std::string & name) const;
    Entity::Ref getChildByType(const std::string & name) const;
    Entity::Ref addChild(Entity::Ref child); // Don't use directly
    u32 removeChild(Entity::Ref child); // Don't use directly
    Entity::Ref createChild(const std::string & typeName = "");
    Entity::Ref requireChild(const std::string & typeName);
    u32 indexOfChild(Entity::Ref child) const;
    inline bool containsChild(const Entity::Ref child) const { return indexOfChild(child) != m_children.size(); }

    // Convenience
    template <class Entity_T>
    std::shared_ptr<Entity_T> createChild()
    {
        Entity::Ref e = createChild(Entity_T::__sGetClassName());
        if (e)
            return std::static_pointer_cast<Entity_T>(e);
        else
            return nullptr;
    }

    // Convenience
    template <class Entity_T>
    std::shared_ptr<Entity_T> getChild()
    {
        std::shared_ptr<Entity> e = getChildByType(Entity_T::__sGetClassName());
        if (e)
            return std::static_pointer_cast<Entity_T>(e);
        else
            return nullptr;
    }

    void destroyChildren();

    virtual void destroy();
    virtual void destroyLater();

    //---------------------------------------------
    // Serialization
    //---------------------------------------------

    static void serialize(JsonBox::Value & o, Entity::Ref e);
    static Entity::Ref unserialize(JsonBox::Value & o, Entity::Ref parent);

    virtual void serializeState(JsonBox::Value & o);
    virtual void unserializeState(JsonBox::Value & o);

    //---------------------------------------------
    // Integrated callbacks
    //---------------------------------------------

    virtual void onReady() {}
    virtual void onDestroy() {}

    virtual void onFirstUpdate() {}
    virtual void onUpdate() {}

private:
    void propagateOnReady();

    u32 removeChild(Entity * child);
    void removeChild(u32 index);
    //Entity::Ref getChildByPointer(Entity * child);

private:
    std::bitset<8> m_flags;
    std::string m_name;
    std::vector<Entity::Ref> m_children; // Owned by the entity
    Entity::WeakRef r_parent; // Not owned by the entity
    mutable SceneWeakRef r_scene; // Not owned by the entity
    std::unordered_set<std::string> m_tags;
};

} // namespace sn

#endif // __HEADER_SN_ENTITY__


