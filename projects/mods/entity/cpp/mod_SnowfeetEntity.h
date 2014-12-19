#ifndef __HEADER_SN_SNOWFEET_ENTITY__
#define __HEADER_SN_SNOWFEET_ENTITY__

#include <core/app/ModuleArgs.hpp>
#include <core/reflect/ObjectTypeDatabase.hpp>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModEntity(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModEntity(ModuleUnloadArgs args);
}

namespace sn {
namespace entity {

void registerNativeTypes(ObjectTypeDatabase & tdb);

} // namespace entity
} // namespace sn

#endif __HEADER_SN_SNOWFEET_ENTITY__

