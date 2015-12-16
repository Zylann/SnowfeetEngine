#ifndef __HEADER_SN_UPDATEMANAGER__
#define __HEADER_SN_UPDATEMANAGER__

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <core/scene/UpdateLayer.h>

namespace sn
{

class Scene;
class Entity;

//class IUpdatable
//{
//public:
//    virtual ~IUpdatable() {}
//    virtual void onUpdate() {}
//};

/// \brief Triggers onUpdate() calls on entities that need it, in a defined order.
/// It is based on tags. For instance, if a layer is named MyUpdatable,
/// all entities having the tag MyUpdatable will have their onUpdate() method called.
class UpdateManager
{
public:
    /// \brief Name of the default update layer
    static const char * DEFAULT_LAYER;

    UpdateManager();
    ~UpdateManager();

    /// \brief Adds an entity to the update cycle. If the layer doesn't exists, it will be created.
    /// \param e: entity
    /// \param layerName: name of the layer to be added into.
    void addEntity(Entity & e, const std::string layerName = DEFAULT_LAYER);

    /// \brief Removes an entity from the update cycle.
    /// \param e: entity
    void removeEntity(Entity & e, const std::string layerName = "");

    /// \brief Calls update on the objects in defined order
    void update();

    /// \brief Removes all entities from update and clears all layers
    void clear();

    bool getEntityLayer(Entity & e, std::string & out_layerName);

    void serialize(Variant & o);
    void unserialize(const Variant & o);

private:
    UpdateLayer * getLayer(const std::string & name) const;
    void updateLayer(const UpdateLayer & layer);

private:
    /// \brief Ordered layers
    std::vector<UpdateLayer*> m_layers;

    /// \brief Layer name => entities
    std::unordered_map< std::string, std::unordered_set<Entity*> > m_updatableEntities;

};

} // namespace sn

#endif // __HEADER_SN_UPDATEMANAGER__

