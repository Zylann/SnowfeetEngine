#ifndef __HEADER_SN_AS_ENTITY_SYSTEM__
#define __HEADER_SN_AS_ENTITY_SYSTEM__

#include <core/app/ScriptEngine.hpp>

namespace sn
{

void register_entitySystem(asIScriptEngine & e);

void declare_Scene(asIScriptEngine & e);
void declare_Entity(asIScriptEngine & e);

void register_Scene(asIScriptEngine & e);
void register_Entity(asIScriptEngine & e);

} // namespace sn

#endif // __HEADER_SN_AS_ENTITY_SYSTEM__

