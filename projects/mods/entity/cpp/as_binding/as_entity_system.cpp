#include "as_entity_system.hpp"

namespace sn
{

void register_entitySystem(asIScriptEngine & e)
{
    // Declare types

    declare_Scene(e);
    declare_Entity(e);

    // Register types

    register_Scene(e);
    register_Entity(e);
}

} // namespace sn

