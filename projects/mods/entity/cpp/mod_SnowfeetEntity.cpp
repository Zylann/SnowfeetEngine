#include "mod_SnowfeetEntity.hpp"
#include "Transform.hpp"
#include "ASBehaviour.hpp"

void registerSnowfeetModule(asIScriptEngine * engine)
{
    // TODO registering
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


