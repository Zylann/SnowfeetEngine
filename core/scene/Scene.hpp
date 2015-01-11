#ifndef __HEADER_SN_SCENE__
#define __HEADER_SN_SCENE__

#include <core/scene/Entity.hpp>
#include <core/util/RefCounted.hpp>
#include <map>

namespace sn
{

/// \brief The scene is the top-level object of the game world.
/// It cannot have a parent and cannot be destroyed.
/// It exists as long as the application runs.
class Scene : public Entity
{
public:

    SN_ABSTRACT_OBJECT(sn::Scene, sn::Object)

    Scene() : Entity()
    {}

    void registerUpdatableEntity(Entity * e, s16 order, s16 layer);
    void unregisterUpdatableEntity(Entity * e);

    void registerTaggedEntity(Entity * e, const std::string & tag);
    void unregisterTaggedEntity(Entity * e, const std::string & tag);

    void setParent(Entity * newParent);

    void destroy() override;
    void destroyLater() override;

    void onUpdate() override;

private:
    std::unordered_map<std::string, std::unordered_set<Entity*>> m_taggedEntities;
    std::map<s32, std::unordered_set<Entity*>> m_updatableEntities;
};

} // namespace sn

#endif // __HEADER_SN_SCENE__


