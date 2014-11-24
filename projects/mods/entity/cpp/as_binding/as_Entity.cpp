#include "as_entity_system.hpp"
#include "../Entity.hpp"

namespace sn
{

static bool Entity_isSticky(Entity * self)
{
    return self->hasFlag(EF_STICKY);
}

static Entity * Entity_getParent(Entity * self)
{
    return self->parent();
}

void declare_Entity(asIScriptEngine & e)
{
    const char * t = "Entity";

    // Don't count references, only the Scene can destroy entities
    asCheck(e.RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT));

    // No constructor (use scene.createEntity())

    // Refcount
    //asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_ADDREF, "void f()", asMETHOD(Entity, addRef), asCALL_THISCALL));
    //asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_RELEASE, "void f()", asMETHOD(Entity, release), asCALL_THISCALL));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD(Entity, getWeakRefFlag), asCALL_THISCALL));
}

void register_Entity(asIScriptEngine & e)
{
    const char * t = "Entity";

    // Methods

    asCheck(e.RegisterObjectMethod(t, "string get_name()", asMETHOD(Entity, getName), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "void set_name(const string &in s)", asMETHOD(Entity, setName), asCALL_THISCALL));

    asCheck(e.RegisterObjectMethod(t, "Scene@ get_scene()", asMETHOD(Entity, getName), asCALL_THISCALL));

    asCheck(e.RegisterObjectMethod(t, "void destroyLater()", asMETHOD(Entity, destroyLater), asCALL_THISCALL));

    asCheck(e.RegisterObjectMethod(t, "bool get_sticky()", asFUNCTION(Entity_isSticky), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "void set_sticky(bool b)", asMETHOD(Entity, setSticky), asCALL_THISCALL));

    asCheck(e.RegisterObjectMethod(t, "uint get_childCount()", asMETHOD(Entity, childCount), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Entity@ getChild(uint i)", asMETHOD(Entity, child), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Entity@ get_parent()", asFUNCTION(Entity_getParent), asCALL_CDECL_OBJLAST));
}

} // namespace sn

