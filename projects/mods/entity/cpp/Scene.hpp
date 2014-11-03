/*
Scene.hpp
Copyright (C) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SCENE__
#define __HEADER_SN_SCENE__

#include <list>
#include <unordered_set>

#include <core/system/time/Clock.hpp>
#include <core/math/Vector3.hpp>

#include "Entity.hpp"
#include "LayerMap.hpp"
#include "ComponentList.hpp"
#include "Behaviour.hpp"
#include "TagManager.hpp"

namespace sn
{

/// \brief A scene contains all stuff that the player can see and interact with.
class SN_API Scene
{
public:

    Scene();
    ~Scene();

    //----------------------------
    // Lifecycle
    //----------------------------

    // TODO add scene modes : EDIT / PLAY / RECORD

    /// \brief Updates the scene for a given time span.
    void update(sn::Time deltaTime);

    /// \brief Erases everything in the scene.
    void clear();

    /// \brief Erases everything in the scene but entities having the EF_SCENE_CROSS flag set.
    void clearAllButCrossEntities();

    //----------------------------
    // State & entities
    //----------------------------

    /// \brief Creates an empty entity, adds it to the scene and returns it.
    /// \note: by default, the update and draw order are defined by their creation order.
    Entity * createEntity(std::string name="", sn::Vector3f pos = sn::Vector3f());

    /// \brief Sets the given camera as the main camera
    //inline void setMainCamera(Camera * cam) { r_mainCamera = cam; }

    /// \brief Gets the main camera (if defined).
    //inline Camera * mainCamera() { return r_mainCamera; }

    /// \brief Finds the entity having the given ID.
    /// \param id: numeric unique identifier of the entity within the scene
    /// \return Returns nullptr when not found.
    Entity * findEntityFromID(u32 id) const;

    /// \brief Finds the first encountered entity having the given name.
    /// \param name: name to search
    /// \return nullptr when not found.
    Entity * findEntityFromName(const std::string & name);

    //----------------------------
    // Events
    //----------------------------

    /// \brief Called each time the screen resolution changes
    /// \param resolution: new resolution in pixels
    //void onScreenResized(sf::Vector2u resolution);

    //----------------------------
    // Metadata
    //----------------------------

    /// \brief Convenient layer name associations
    LayerMap layers;

    /// \brief Tag definitions
    TagManager tagManager;

    /// \brief Returns the amount of time elapsed since the scene was loaded
    inline sn::Time time() const { return m_time.getElapsedTime(); }

    /// \brief Returns the duration between two frames
    inline sn::Time deltaTime() const { return m_deltaTime; }

    //----------------------------
    // Serialization
    //----------------------------

    /// \brief saves the scene in a JSON data tree.
    void serialize(JsonBox::Value & o);
    /// \brief loads the scene from a JSON data tree.
    void unserialize(JsonBox::Value & o);

    /// \brief saves the scene in a JSON file.
    /// \param filePath: path to the file. If it exists, it is overwritten. If it doesn't exists, it is created.
    /// \return true if success, false if an error occurred.
    bool saveToFile(const std::string & filePath);
    /// \brief loads the scene from a JSON file.
    /// \param filePath: path to the file.
    /// \return true if success, false if the file didn't exist or had invalid structure.
    bool loadFromFile(const std::string & filePath);

    //----------------------------
    // Component systems
    //----------------------------

    /// \brief Internal: called when a behaviour has been created
    void registerBehaviour(Behaviour * behaviour);

    /// \brief Internal: called just before a behaviour to be deleted
    void unregisterBehaviour(Behaviour * behaviour);

    /// \brief All the animators in the scene.
    //ComponentList<Animator> animators;
    /// \brief Manages physics in the scene (all colliders, all bodies).
    //PhysicsSystem physics;
    /// \brief Controls global audio in the scene (also references AudioEmitters).
    //AudioSystem audioSystem;
    /// \brief Everything visual (list of renderers and cameras).
    //RenderSystem renderSystem;
    /// \brief GUI system.
    //GUIManager guiManager;

    //------------------------------
    // Misc. settings
    //------------------------------

    /// \brief If set to true, a built-in 3D Transform component will be automatically added
    /// to each entity created in this scene.
    bool automaticTransforms;

//protected:

    /// \brief draws the entire scene on the given target.
    //void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:

    /// \brief counter used to generate entity UIDs
    u32 m_nextID;

    // TODO change to a vector?
    /// \brief list of all entities in the scene.
    std::list<Entity*> m_entities;

    /// \brief References to all behaviours [updateOrder][index]
    std::map< s32, ComponentList<Behaviour> > m_behaviours;

    /// \brief reference to the main camera
    //Camera * r_mainCamera;

    /// \brief time elapsed since the scene was loaded
    sn::Clock m_time;

    /// \brief Cached current frame delta time
    sn::Time m_deltaTime;

};

} // namespace sn

#endif // __HEADER_SN_SCENE__



