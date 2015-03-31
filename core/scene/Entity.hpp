/*
Entity.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ENTITY__
#define __HEADER_SN_ENTITY__

#include <core/types.hpp>
#include <core/json/json_utils.hpp>
#include <core/app/ScriptObject.hpp>
#include <core/system/gui/Event.hpp>
#include <core/scene/ScriptInstance.hpp>
#include <core/asset/SerializationContext.hpp>

#include <vector>
#include <string>
#include <bitset>
#include <unordered_set>
#include <functional>

#define SN_ENTITY(_className, _baseName)                                   \
    SN_SCRIPT_OBJECT(_className, _baseName)

namespace sn
{

enum EntityFlags
{
    SN_EF_ENABLED = 0,
    SN_EF_DESTROYED = 1,
    SN_EF_FIRST_UPDATE = 2,
    SN_EF_UPDATABLE = 3,
    SN_EF_STICKY = 4,
	SN_EF_SYSTEM_EVENT_LISTENER = 5
};

class Scene;

/// \brief Base class for all components of the scene.
class SN_API Entity : public ScriptObject
{
public:

    SN_ENTITY(sn::Entity, sn::ScriptObject)

    Entity() :
        ScriptObject(),
        r_parent(nullptr),
        r_scene(nullptr)
    {}

    inline const std::string & getName() const { return m_name; }
    void setName(const std::string & name);

    inline bool getFlag(u32 position) const { return m_flags.at(position); }
    void setFlag(u32 position, bool v);

    /// \brief Tells if the entity is enabled
    bool isEnabled() const;
    /// \brief Tells if the entity is enabled without looking at its parent enabled state
    inline bool isEnabledSelf() const { return m_flags.at(SN_EF_ENABLED); }
    /// \brief Enables the entity (the parent overrides this if it is disabled).
    void setEnabled(bool e);

    void setUpdatable(bool enable=true, s16 order=0, s16 layer=0);
	void listenToSystemEvents(bool enable=true);

    inline bool hasTag(const std::string & tag) const { return m_tags.find(tag) != m_tags.end(); }
    void addTag(const std::string & tag);
    void removeTag(const std::string tag); // not a reference because tag could refer to a string in the tag list

    std::string toString() const;

    inline Entity * getParent() const { return r_parent; }
    virtual void setParent(Entity * newParent);
    Entity * getRoot() const;

    Scene * getScene() const;

    inline u32 getChildCount() const { return m_children.size(); }

    /// \brief Gets a child by its index.
    /// \warning These indexes only deserve the purpose of iterating.
    /// They are only guaranteed to be constant as long as you don't add or remove children.
    /// They shouldn't be used as stable references.
    inline Entity * getChildByIndex(u32 i) const { return m_children[i]; }

    Entity * getChildByName(const std::string & name) const;
    Entity * getChildByType(const std::string & name) const;
    Entity * addChild(Entity * child); // Don't use directly
    u32 removeChild(Entity * child); // Don't use directly
    Entity * createChildNoParams() { return createChild(); }
    Entity * createChild(const std::string & typeName = "");
    Entity * requireChild(const std::string & typeName);
    u32 indexOfChild(const Entity * child) const;
    inline bool containsChild(const Entity * child) const { return indexOfChild(child) != m_children.size(); }

    // Convenience
    template <class Entity_T>
    Entity_T * createChild()
    {
        Entity * e = createChild(Entity_T::__sGetClassName());
        if (e)
            return static_cast<Entity_T*>(e);
        else
            return nullptr;
    }

    // Convenience
    template <class Entity_T>
    Entity_T * getChild()
    {
        Entity * e = getChildByType(Entity_T::__sGetClassName());
        if (e)
            return static_cast<Entity_T*>(e);
        else
            return nullptr;
    }

    /// \brief Destroys all children of this entity
    void destroyChildren();

    /// \brief Destroys all children of this entity fulfilling a condition
    void destroyChildren(std::function<bool(const Entity&)> f_mustBeDestroyed);

    /// \brief Same as release(), but notifies children and parent before calling release().
    virtual void destroy();
    /// \brief Schedules a destroy() at the end of the current update.
    virtual void destroyLater();

    //---------------------------------------------
    // Serialization
    //---------------------------------------------

    static void serialize(JsonBox::Value & o, Entity & e, const SerializationContext & context);
    static Entity * unserialize(JsonBox::Value & o, Entity * parent, const SerializationContext & context);

    virtual void serializeState(JsonBox::Value & o, const SerializationContext & context);
    virtual void unserializeState(JsonBox::Value & o, const SerializationContext & context);

    //---------------------------------------------
    // Integrated callbacks
    //---------------------------------------------

    virtual void onReady() {}
    virtual void onDestroy() {}

    virtual void onFirstUpdate() {}
    virtual void onUpdate() {}

	// Returns true if the event has been consumed, true otherwise
	virtual bool onSystemEvent(const sn::Event & event) { return false; }

protected:
    virtual ~Entity();

private:
    void propagateOnReady();
    void removeChildAtIndex(u32 index);

private:
    std::bitset<8> m_flags;
    std::string m_name;
    std::vector<Entity*> m_children; // Owned by the entity
    Entity * r_parent; // Not owned by the entity
    mutable Scene * r_scene; // Not owned by the entity
    std::unordered_set<std::string> m_tags;
    ScriptInstance m_script;

};

} // namespace sn

#endif // __HEADER_SN_ENTITY__


