/*
Entity.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ENTITY__
#define __HEADER_SN_ENTITY__

#include <core/types.h>
#include <core/sml/variant_serialize.h>
#include <core/app/ScriptableObject.h>
#include <core/system/Event.h>
#include <core/squirrel/Instance.h>
#include <core/asset/SerializationContext.h>
#include <core/util/typecheck.h>
#include <core/scene/UpdateManager.h>
#include <core/util/Indexer.h>

#include <vector>
#include <string>
#include <bitset>
#include <unordered_set>
#include <functional>

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
class Entity;

typedef Indexer<Entity*> EntityIndexer;
typedef EntityIndexer::Key EntityID;

/// \brief Base class for all components of the scene.
class SN_API Entity : public ScriptableObject
{
public:
	SN_OBJECT

	static const u32 MAX_TAGS = 32;

    /// \brief Just constructs the entity.
    /// \note As most of serialized classes in the engine, 
    /// subclasses shouldn't declare a constructor with parameters.
    Entity();

    //---------------------------------------------
    // State
    //---------------------------------------------

    /// \brief Gets the runtime ID of the entity.
    /// It is unique within the scope and lifetime of the scene.
    inline EntityID getId() const { return m_id; }

    /// \brief Gets the name of the entity
    inline const std::string & getName() const { return m_name; }
    /// \brief Sets the name of the entity
    void setName(const std::string & name);

    /// \brief Gets a flag of the entity
    inline bool getFlag(u32 position) const { return m_flags.at(position); }
    /// \brief Sets a flag on the entity.
    /// \warning This doesn't triggers any notification system.
    /// Some flags (such as UPDATABLE) must be set using specialized methods.
    void setFlag(u32 position, bool v);

    /// \brief Tells if the entity is enabled
    bool isEnabled() const;
    /// \brief Tells if the entity is enabled without looking at its parent enabled state
    inline bool isEnabledSelf() const { return m_flags.at(SN_EF_ENABLED); }
    /// \brief Enables the entity (the parent overrides this if it is disabled).
    void setEnabled(bool e);

    /// \brief Sets wether this entity should be included in the onUpdate() process.
    /// \param enable: make updatable or disable update
    /// \param order: update order within the update layer
    /// \param layer: update layer ID
    void setUpdatable(bool enable, const std::string layerName = "");

    /// \brief Sets wether this entity should be notified of system events.
    /// \brief enable: enables notification or disables it
	void listenToSystemEvents(bool enable=true);

    /// \brief Tests if the entity has a tag set on it
    /// \param tag: tag name to test
	inline bool hasTag(const std::string & tag) const;
    /// \brief Adds a tag on the entity by its name
    /// \param tag: name of the tag to add
    void addTag(const std::string & tag);
    /// \brief Removes a tag from the entity by its name
    /// \param tag: name of the tag to remove
    /// \note not a reference because tag it refer to a string in the tag list
    void removeTag(const std::string tag);

    /// \brief Gets a short, human-readable string representation of the entity
    std::string toString() const;

    //---------------------------------------------
    // Hierarchy
    //---------------------------------------------

    /// \brief Gets the parent entity. Returns nullptr if it has no parent.
    inline Entity * getParent() const { return r_parent; }
    /// \brief Sets the parent entity.
    /// If the entity already has a parent, it is unparented first.
    /// \param newParent: the parent to set. If nullptr, the entity will be moved to the root.
    virtual void setParent(Entity * newParent);
    /// \brief Gets the top-most entity in the whole hierarchy this entity is included into.
    Entity * getRoot() const;

    /// \brief Gets the first parent Scene in the hierarchy this entity is included into.
    Scene * getScene() const;

    /// \brief Gets the number of children this entity has
    inline u32 getChildCount() const { return m_children.size(); }

    /// \brief Gets a direct child by its index.
    /// \warning These indexes only deserve the purpose of iterating.
    /// They are only guaranteed to be constant as long as you don't add or remove children.
    /// They shouldn't be used as stable references.
    inline Entity * getChildByIndex(u32 i) const { return m_children[i]; }

    /// \brief Gets a direct child by its name. Returns null if not found.
    Entity * getChildByName(const std::string & name) const;
    /// \brief Gets a direct child by its type name. Returns null if not found.
    Entity * getChildByType(const ObjectType & type) const;

    u32 getIndexInParent() const;

    /// \brief Creates a child entity of the given type.
    /// \param typeName: name of the type this entity must be
    /// \return The new entity
    Entity * createChild(const ObjectType & type);
    /// \brief Creates child entity of the given type if it is not present
    /// \param typeName: name of the type this entity must have
    /// \return The first child entity of the given type, or a newly created one
    Entity * requireChild(const ObjectType & type);
    /// \brief Tests if the entity contains the given entity as a direct child.
    /// \param child: entity to find
    inline bool containsChild(const Entity * child) const { return indexOfChild(child) != m_children.size(); }

    /// \brief Creates a child entity of the given compile-time type.
    /// \return The new entity
    template <class Entity_T>
    Entity_T * createChild()
    {
        Entity * e = createChild(sn::getObjectType<Entity_T>());
        if (e)
            return static_cast<Entity_T*>(e);
        else
            return nullptr;
    }

    /// \brief Gets the first direct child entity of the given compile-time type.
    /// \return The new entity
    template <class Entity_T>
    Entity_T * getChild()
    {
        Entity * e = getChildByType(sn::getObjectType<Entity_T>());
        if (e)
            return static_cast<Entity_T*>(e);
        else
            return nullptr;
    }

    template <class Entity_T>
    void getChildrenOfType(std::vector<Entity_T*> & out) const
    {
        for (u32 i = 0; i < m_children.size(); ++i)
        {
            Entity * e = m_children[i];
            if (e->isInstanceOf<Entity_T>())
                out.push_back(checked_cast<Entity_T*>(e));
        }
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

    /// \brief Serializes the state of the entity inside a JSON node.
    /// This method is intented to be overrided in sub-classes to add specialized datas.
    /// (don't forget to call the base's version!)
    /// \param o: JSON node to fill
    /// \param context: context
    virtual void serializeState(Variant & o, const SerializationContext & context);
    /// \brief Serializes the state of the entity inside a JSON node.
    /// This method is intented to be overrided in sub-classes to read specialized datas.
    /// (don't forget to call the base's version!)
    /// \param o: JSON node to fill
    /// \param context: context
    virtual void unserializeState(const Variant & o, const SerializationContext & context);

    //---------------------------------------------
    // Integrated callbacks
    //---------------------------------------------

    /// \brief Called when the entity and all its children entered the scene.
	/// The calling order on children is undefined.
    virtual void onReady();

    /// \brief Called when the entity is marked (or scheduled) as destroyed.
    /// Destruction usually follow immediately after, or at the end of the update cycle.
    virtual void onDestroy();

    /// \brief Called just before the first call to onUpdate().
    virtual void onFirstUpdate();

    /// \brief Called on each update cycle if the entity has been marked as updatable.
    virtual void onUpdate();

    /// \brief Called when a system event occurs if this entity has been marked as a system event listener.
	/// \return true if the event has been consumed, false otherwise
	virtual bool onSystemEvent(const sn::Event & event) { return false; }

	//---------------------------------------------
    // Scripting
    //---------------------------------------------

	squirrel::Instance & getScript() { return m_script; }

    //---------------------------------------------
    // Internal use
    //---------------------------------------------

    /// \brief Adds a child to this entity without checks or notifications.
    /// \warning Don't use directly, use setParent() instead.
    Entity * addChild(Entity * child);
    /// \brief Removes a child from this entity without checks or notifications.
    /// \warning Don't use directly, use setParent() instead.
    u32 removeChild(Entity * child);

    void propagateOnReady();

    static void debugPrintEntityTree(Entity & e);

protected:
    virtual ~Entity();

private:
    /// \brief Gets the index of the given entity within the internal child container.
    /// \return the index, or the size of the container if not found.
    u32 indexOfChild(const Entity * child) const;

    void removeChildAtIndex(u32 index);

    void releaseScript();

    void onSceneChanged(Scene * oldScene, Scene * newScene);

    void removeAllTags();

	void getTags(std::vector<std::string> & tags);

private:
    /// \brief Flags of the entity.
    /// These are like internal tags used on the engine's core side.
    std::bitset<8> m_flags;

    /// \brief User-defined name of the entity.
    std::string m_name;

    /// \brief Children owned by the entity.
    std::vector<Entity*> m_children;

    /// \brief Parent of this entity. Can be null.
    Entity * r_parent;

    /// \brief Scene this entity is included into. Can be null.
    mutable Scene * r_scene;

    /// \brief User-defined tags currently set on this entity.
	/// Each bit corresponds to a tag in the scene this entity is registered.
	std::bitset<MAX_TAGS> m_tags;

    // TODO Components.
    /// \brief Script behaviour attached to this entity. Can be unset.
    squirrel::Instance m_script;

    /// \brief Runtime ID
    EntityID m_id;

};

} // namespace sn

#endif // __HEADER_SN_ENTITY__


