#include "as_math.hpp"
#include "../Vector2.hpp"
#include "../../app/ScriptEngine.hpp"

namespace sn
{

//------------------------------------------------------------------------------
static void Vector2i_defaultConstructor(Vector2i *self)
{
    new(self)Vector2i();
}

//------------------------------------------------------------------------------
static void Vector2i_copyConstructor(const Vector2i &other, Vector2i *self)
{
    new(self)Vector2i(other);
}

//------------------------------------------------------------------------------
static void Vector2i_initConstructor(s32 x, s32 y, Vector2i *self)
{
    new(self)Vector2i(x, y);
}

//------------------------------------------------------------------------------
static void Vector2i_listConstructor(s32 *list, Vector2i *self)
{
    new(self)Vector2i(list[0], list[1]);
}

//------------------------------------------------------------------------------
void register_Vector2i(asIScriptEngine * e)
{
    const char * t = "Vector2i";

    // Register type
    asCheck(e->RegisterObjectType(t, sizeof(Vector2i), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Fields
    asCheck(e->RegisterObjectProperty(t, "int x", asOFFSET(Vector2i, x)));
    asCheck(e->RegisterObjectProperty(t, "int y", asOFFSET(Vector2i, y)));

    // Constructors
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vector2i_defaultConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const Vector2i &in)", asFUNCTION(Vector2i_copyConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(Vector2i_initConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {int, int}", asFUNCTION(Vector2i_listConstructor), asCALL_CDECL_OBJLAST));

    // Register the operator overloads
    asCheck(e->RegisterObjectMethod(t, "Vector2i &opAddAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator+=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i &opSubAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator-=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i &opMulAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator*=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i &opDivAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator/=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "bool opEquals(const Vector2i &in) const", asMETHODPR(Vector2i, operator==, (const Vector2i &) const, bool), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i opAdd(const Vector2i &in) const", asMETHODPR(Vector2i, operator+, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i opSub(const Vector2i &in) const", asMETHODPR(Vector2i, operator-, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i opMul(const Vector2i &in) const", asMETHODPR(Vector2i, operator*, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Vector2i opDiv(const Vector2i &in) const", asMETHODPR(Vector2i, operator/, (const Vector2i &) const, Vector2i), asCALL_THISCALL));

}

} // namespace sn


