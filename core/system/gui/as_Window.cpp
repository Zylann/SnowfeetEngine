#include "../../app/ScriptEngine.hpp"
#include "as_systemgui.hpp"

namespace sn
{

void register_Window(asIScriptEngine * engine)
{
    // Registering the reference type
    asCheck(engine->RegisterObjectType("Window", 0, asOBJ_REF));

}

} // namespace sn


