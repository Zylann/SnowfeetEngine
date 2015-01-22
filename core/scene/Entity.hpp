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
#include <memory>

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

class SN_API Entity : public RefCounted, public Object
{
public:

    SN_OBJECT(sn::Entity, sn::Object);

    Entity() :
        RefCounted(),
        Object(),
        r_parent(nullptr),
        r_scene(nullptr)
    {}

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

    inline Entity * getParent() const { return r_parent; }
    virtual void setParent(Entity * newParent);
    Entity * getRoot() const;

    Scene * getScene() const;

    inline u32 getChildCount() const { return m_children.size(); }
    inline Entity * getChildByIndex(u32 i) const { return m_children[i]; }
    Entity * getChildByName(const std::string & name) const;
    Entity * getChildByType(const std::string & name) const;
    Entity * addChild(Entity * child);
    u32 removeChild(const Entity * child);
    Entity * createChild(const std::string & typeName="");
    Entity * requireChild(const std::string & typeName);
    u32 indexOfChild(const Entity * child) const;
    inline bool containsChild(const Entity * child) const { return indexOfChild(child) != m_children.size(); }

    template <class Entity_T>
    Entity_T * createChild()
    {
        Entity * e = createChild(Entity_T::__sGetClassName());
        if (e)
            return (Entity_T*)e;
        else
            return nullptr;
    }

    template <class Entity_T>
    Entity_T * getChild()
    {
        Entity * e = getChildByType(Entity_T::__sGetClassName());
        if (e)
            return (Entity_T*)e;
        else
            return nullptr;
    }

    void destroyChildren();

    virtual void destroy();
    virtual void destroyLater();

    //---------------------------------------------
    // Serialization
    //---------------------------------------------

    static void serialize(JsonBox::Value & o, Entity & e);
    static Entity * unserialize(JsonBox::Value & o, Entity & parent);

    virtual void serializeState(JsonBox::Value & o);
    virtual void unserializeState(JsonBox::Value & o);

    //---------------------------------------------
    // Integrated callbacks
    //---------------------------------------------

    virtual void onReady() {}
    virtual void onDestroy() {}

    virtual void onFirstUpdate() {}
    virtual void onUpdate() {}

protected:
	virtual ~Entity()
	{
		if (r_parent)
			r_parent->removeChild(this);
		destroyChildren();
		for (auto it = m_tags.begin(); it != m_tags.end(); ++it)
			removeTag(*it);
	}

private:
    void propagateOnReady();

private:
    std::bitset<8> m_flags;
    std::string m_name;
    std::vector<Entity*> m_children;
    Entity * r_parent;
    mutable Scene * r_scene;
    std::unordered_set<std::string> m_tags;
};

} // namespace sn

#endif // __HEADER_SN_ENTITY__


