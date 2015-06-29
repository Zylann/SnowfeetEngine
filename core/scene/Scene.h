/*
Scene.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SCENE__
#define __HEADER_SN_SCENE__

#include <core/scene/Entity.h>
#include <core/util/RefCounted.h>
#include <core/system/gui/Event.h>
#include <core/system/time/Clock.h>
#include <map>

namespace sn
{

// SUGG: getScene() return the first parent scene, getRoot() returns the top-most,
// that can be either the game scene or editor's scene?

/// \brief The scene is the top-level object of the game world.
/// It cannot have a parent and cannot be destroyed.
/// Its semantic is like an Application, it exists as long as the program runs.
/// (Each entity is a scene, in some way. But the actual Scene is the only one root).
/// \note: you can still nest scenes by creating some in your entities, to make an editor for instance.
/// However, sub-scenes cannot interact with their parent unless custom access is given.
class SN_API Scene : public Entity
{
public:

    SN_ENTITY(sn::Scene, sn::Entity)

    Scene() : 
        Entity(), 
        m_quitFlag(false)
    {
        setName("Scene");
    }

    //------------------------------------
    // Registers
    //------------------------------------

    void registerUpdatableEntity(Entity & e, s16 order, s16 layer);
    void unregisterUpdatableEntity(Entity & e);

    void registerTaggedEntity(Entity & e, const std::string & tag);
    void unregisterTaggedEntity(Entity & e, const std::string & tag);

	void registerEventListener(Entity & e);
	void unregisterEventListener(Entity & e);

    /// \brief Returns the first encountered entity having the given tag.
    /// \param tag: tag to search
    /// \return an entity with the tag, or nullptr if none have it
    Entity * getTaggedEntity(const std::string & tag);

    /// \brief Returns all entities having the given tag.
    /// \param tag: tag to search
    /// \return list of all entities having the tags. Can be empty.
    std::vector<Entity*> getTaggedEntities(const std::string & tag);

    //------------------------------------
    // Entity overrides
    //------------------------------------

    void setParent(Entity * newParent) override;

    void destroy() override;
    void destroyLater() override;

    //------------------------------------
    // Events
    //------------------------------------

    void onUpdate() override;

    bool onSystemEvent(const Event & ev);

    inline void quit() { m_quitFlag = true; }
    inline bool getQuitFlag() const { return m_quitFlag; }

    //------------------------------------
    // Scene-specific
    //------------------------------------

	void update(Time deltaTime);

    /// \brief Destroys all children entities, except sticky entities (services).
    /// Use destroyChildren() if you want to destroy everything.
    void destroyChildrenButServices();

	Time getDeltaTime() const { return m_deltaTime; }
	Time getTimeSinceStartup() const;

    //------------------------------------
    // Serialization
    //------------------------------------

    void loadFromFile(const std::string & filePath, const SerializationContext & context);
    void saveToFile(const std::string & filePath, const SerializationContext & context);

private:
    std::unordered_map<std::string, std::unordered_set<Entity*>> m_taggedEntities;
    std::map<s32, std::unordered_set<Entity*>> m_updatableEntities;
	std::unordered_set<Entity*> m_eventListenerEntities;
    bool m_quitFlag;
	Time m_deltaTime;
	Clock m_timeClock;

};

} // namespace sn

#endif // __HEADER_SN_SCENE__


