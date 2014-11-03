/*
Entity.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ENTITY__
#define __HEADER_SN_ENTITY__

#include <vector>
#include <bitset>

#include <core/types.hpp>
#include <core/util/typecheck.hpp>
#include <core/reflect/ObjectTypeDatabase.hpp>
#include <JsonBox.h>

#include "LayerMap.hpp"

namespace sn
{

class Component;
class Scene;

/// \brief Flags that can be set on the entity, as bitmasks.
enum EntityFlags
{
    EF_ACTIVE       = 0,
    EF_DESTROY_LATE = 1, // Destroy at the end of the frame
    EF_CROSS_SCENE  = 2 // Don't destroy the entity on scene change
};

/// \brief An entity can be anything in the scene.
/// It is composed of components that define its appearance, interactions and behavior.
class SN_API Entity
{
public:

    /// \brief Constructs a new empty entity, with no link to a scene.
    /// \warning You usually shouldn't call this constructor, use Scene::createEntity() instead.
    /// \see Scene.hpp
    Entity();

    /// \brief Destroys the entity and all of its components
    ~Entity();

    //--------------------------------------
    // Lifecycle
    //--------------------------------------

    /// \brief Tests if the entity alone is active, regardless of its parents
    /// \return true if the entity alone is active, false otherwise
    bool active() const;

    /// \brief Returns true if the entity is active and all of its parents are active.
    /// \return true if the entity is active, false otherwise
    bool activeInHierarchy() const;

    /// \brief Activates or desactivates the entity.
    /// \param active
    void setActive(bool active);

    /// \brief Schedules the destruction of the entity at the end of the current frame.
    /// \note this will also destroy any child entity under this one at _destruction_time_.
    /// For instance, if you unparent a child before the end of the frame, this
    /// child will not be deleted.
    void destroyLater();

    /// \brief Sets if the entity should be destroyed when we change the scene
    void setCrossScene(bool crossScene);

    /// \brief Gets the value of a flag on the entity by providing its mask.
    /// \return wether the flag is set or not
    inline bool flag(u8 flagIndex) const { return m_flags.test(flagIndex); }

    //--------------------------------------
    // Structure & behavior
    //--------------------------------------

    /// \brief gets the name of the entity.
    /// \return name of the entity.
    inline const std::string & name() const { return m_name; }

    /// \brief Sets the name of the entity. There is no particular restrictions on it,
    /// except that it may be short enough to identifiate the entity well for humans.
    /// \param newName: name to give.
    inline void setName(const std::string & newName) { m_name = newName; }

    /// \brief Adds a native component of the given type to the entity.
    /// \note Note that an entity cannot have two components of the same type.
    /// \return the newly added component, or nullptr if something went wrong.
    template <class Component_T>
    Component_T * addComponent()
    {
        // Get metaclass
        const ObjectType & ct = Component_T::sObjectType();

        // Check if the component can be added
        if(!checkComponentAddition(ct, "Entity::addComponent"))
        {
            return nullptr;
        }

        // Create component
        Component_T * newComponent = new Component_T();

        // Call internal component adder
        addComponent(newComponent);

        return newComponent;
    }

    // TODO template equivalent for removeComponent()

    /// \brief Removes a component from the entity by providing a direct reference to it.
    /// \param cmp: pointer on the component to remove.
    void removeComponent(Component * cmp);

    /// \brief Finds the component of the given type attached to this entity.
    /// \param includeInheritance: if set to true, the inheritance tree of components
    /// will be tested too in order to find by parent types. However, it makes the search slower.
    /// \return the component, or nullptr if not found.
    template <class Component_T>
    Component_T * getComponent(bool includeInheritance=true) const
    {
        const ObjectType & ct = Component_T::sObjectType();
        Component * cmp = getComponent(ct, includeInheritance);
        if(cmp)
        {
            return checked_cast<Component_T*>(cmp);
        }
        return nullptr;
    }

    /// \brief Finds the component of the given type attached to this entity.
    /// \param cmpType: metaclass of the component to search
    /// \param includeInheritance: if set to true, the inheritance tree of components
    /// will be tested too in order to find by parent types. However, it makes the search slower.
    /// \return the component, or nullptr if not found.
    Component * getComponent(const ObjectType & cmpType, bool includeInheritance=true) const;

    /// \brief Returns the "main" component of the entity, if any
    inline Component * actor() const { return r_actor; }

    /// \brief Sets the "main" component of the entity
    void setActor(Component * actor);

    /// \brief Returns the scene the entity belongs to.
    Scene & scene() const;

    // Sends a message to the components of the entity
    void sendMessage(const std::string & msg);

    //--------------------------------------
    // Hierarchy
    //--------------------------------------

    /// \brief Gets the parent entity, if any.
    /// \return Parent entity, or nullptr
    inline Entity * parent() const { return r_parent; }

    /// \brief Parents the entity to another one.
    /// \param newParent: parent to add the entity to.
    void setParent(Entity * newParent);

    /// \brief Gets the number of immediate children the entity has.
    inline u32 childCount() const { return m_children.size(); }

    /// \brief Gets the child entity at the given index.
    /// \param index: index between 0 and childCount(). Must be valid.
    /// \return Child entity
    Entity & child(u32 index);

    /// \brief Removes immediate children of the entity from its descendant hierarchy
    /// (This is the same as doing setParent(null) on every child).
    void uparentChildren();

    //--------------------------------------
    // Layers
    //--------------------------------------

    /// \brief Moves the entity to the specified layer.
    /// \param layer: the name of the layer
    void setLayerByName(const std::string & layer);

    /// \brief Moves the entity to the specified layer.
    /// \param layer: the numeric index of the layer
    void setLayerByIndex(u32 layerIndex);

    /// \brief Gets the index of the layer the entity belongs to.
    /// \return The numeric index of the layer.
    inline u32 layerIndex() const { return m_layerIndex; }

    /// \brief Gets the layer the entity belongs to.
    /// \return Reference to the layer object.
    const Layer & layer() const;

    //--------------------------------------
    // Tags
    //--------------------------------------

    // Tags can be used to identify specific entities or group of entities like
    // the avatar, the main camera, some ennemies...

    /// \brief Adds a tag to the entity (faster).
    /// \param tagIndex: index of the tag in [0,31]
    void addTag(u8 tagIndex);

    /// \brief Adds a tag to the entity.
    /// \param tagName: name of the tag
    void addTag(const std::string & tagName);

    /// \brief Removes a tag from the entity (faster).
    /// \param tagIndex: index of the tag in [0,31]
    void removeTag(u8 tagIndex);

    /// \brief Removes a tag from the entity.
    /// \param tagName: name of the tag
    void removeTag(const std::string & tagName);

    /// \brief Tests if the entity has the given tag (faster).
    /// \param tagIndex: index of the tag in [0,31]
    /// \return true if it has the tag, false otherwise
    bool hasTag(u8 tagIndex) const;

    /// \brief Tests if the entity has the given tag.
    /// \param tagName: name of the tag
    /// \return true if it has the tag, false otherwise
    bool hasTag(const std::string & tagName) const;

    //--------------------------------------
    // Serialization
    //--------------------------------------

    /// \brief serializes the entity to a JSON data tree.
    /// \param o: JSON object representing the entity
    void serialize(JsonBox::Value & o);

    /// \brief deserializes the entity from a JSON data tree.
    /// \param o: JSON object representing the entity
    void unserialize(JsonBox::Value & o);

    /// \brief must be called after the entity to be deserialized into a scene,
    /// in order to relink references and perform some checks
    /// \param o: JSON object representing the entity, to pick the references data
    void postUnserialize(JsonBox::Value & o);

    /// \brief Gets the unique numerical ID of this entity
    /// \return Numerical ID of the entity
    inline u32 id() const { return m_id; }

    //-----------------------------------------
    // Public attributes
    //-----------------------------------------

private:

    friend class Scene;

    /// \brief Sets the scene the entity belongs to.
    /// Usually, only the scene has to call it once when the entity is created.
    inline void setScene(Scene * scene)
    {
        r_scene = scene;
    }

    /// \brief Adds a component to the entity. Internal use.
    /// \param newCmp: newly created component
    /// \return the component passed in parameter.
    Component * addComponent(Component * newCmp);

    /// \brief Checks a component type before adding a component instance.
    /// Prints debug messages if the check fails.
    /// \param ct: meta-class of the component to add
    /// \param context: contextual information to give if an error occurs (ie. the calling function).
    /// \return true if the component can be added, false otherwise
    bool checkComponentAddition(const ObjectType & ct, const std::string & context);

    //--------------------------------------
    // Attributes
    //--------------------------------------

    /// \brief Unique numerical ID.
    /// It is generated when the entity is created and added to a scene.
    /// \see Scene.cpp
    u32 m_id;

    /// \brief Components attached to this entity
    std::vector<Component*> m_components;

    /// \brief Reference to the parent entity
    Entity * r_parent;

    /// \brief References to children entities
    std::vector<Entity*> m_children;

    /// \brief "main" component of the entity.
    /// It can be left null, depending on your needs.
    Component * r_actor;

    /// \brief Bitset containing boolean states of the entity
    std::bitset<8> m_flags;

    /// \brief Numerical index of the layer the entity belongs to
    u32 m_layerIndex;

    /// \brief Scene the entity belongs to
    Scene * r_scene;

    /// \brief User-defined name of the entity
    std::string m_name;

    /// \brief Tags of the entity as bitset. Each bit represents a tag.
    /// Tags have a name stored in Scene.
    /// \see Scene
    u32 m_tags;

};

} // namespace sn

#endif // __HEADER_SN_ENTITY__


