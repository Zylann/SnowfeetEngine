#ifndef __HEADER_SN_SNOWFEET_ENTITY__
#define __HEADER_SN_SNOWFEET_ENTITY__

#include <angelscript.h>
#include <core/reflect/ObjectTypeDatabase.hpp>

void registerSnowfeetModule(asIScriptEngine * engine);

namespace sn {
namespace entity {

void registerNativeTypes(ObjectTypeDatabase & tdb);

} // namespace entity
} // namespace sn

#endif __HEADER_SN_SNOWFEET_ENTITY__

