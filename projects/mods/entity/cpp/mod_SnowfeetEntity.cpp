#include "mod_SnowfeetEntity.h"
#include "Transform.hpp"
#include "ASBehaviour.hpp"
#include "as_binding/as_entity_system.hpp"

int SN_MOD_EXPORT loadSnowfeetModule_ModEntity(ModuleLoadArgs args)
{
    sn::register_entitySystem(*args.scriptEngine);
    sn::entity::registerNativeTypes(sn::ObjectTypeDatabase::get());
    return 0;
}

int SN_MOD_EXPORT unloadSnowfeetModule_ModEntity(ModuleUnloadArgs args)
{
    return 0;
}

namespace sn {
namespace entity {

void registerNativeTypes(ObjectTypeDatabase & tdb)
{
    //ObjectTypeDatabase & tdb = ObjectTypeDatabase::get();

    // Base

    tdb.registerType<sn::Component>();
    tdb.registerType<sn::AbstractTransform>();
    tdb.registerType<sn::Transform>();

    // Behaviours

    tdb.registerType<sn::Behaviour>();
    tdb.registerType<sn::ASBehaviour>();
}

} // namespace entity
} // namespace sn


