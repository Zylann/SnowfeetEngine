#include "as_entity_system.hpp"
#include "../Scene.hpp"

namespace sn
{

static Scene * Scene_defaultConstructor()
{
    return new Scene();
}

static Entity * Scene_createEntity(const std::string & name, Scene * self)
{
    return self->createEntity(name);
}

void declare_Scene(asIScriptEngine & e)
{
    const char * t = "Scene";

    // Type declaration (scripts can create the Scene)
    asCheck(e.RegisterObjectType("Scene", 0, asOBJ_REF));// | asOBJ_GC));

    // Refcount
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_ADDREF, "void f()", asMETHOD(Scene, addRef), asCALL_THISCALL));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_RELEASE, "void f()", asMETHOD(Scene, release), asCALL_THISCALL));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD(Scene, getWeakRefFlag), asCALL_THISCALL));
}

void register_Scene(asIScriptEngine & e)
{
    const char * t = "Scene";

    // Constructor
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_FACTORY, "Scene@ f()", asFUNCTION(Scene_defaultConstructor), asCALL_CDECL));

    // Methods
    asCheck(e.RegisterObjectMethod(t, "void update(Time delta)", asMETHOD(Scene, update), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Entity@ createEntity(const string &in name)", asFUNCTION(Scene_createEntity), asCALL_CDECL_OBJLAST));

}

} // namespace sn


