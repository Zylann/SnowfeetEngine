#include "as_math.hpp"
#include "../Vector2.hpp"
#include "../../app/ScriptEngine.hpp"

namespace sn
{

//------------------------------------------------------------------------------
template <typename T> static void Vector2_defaultConstructor(Vector2<T> *self)                       { new(self)Vector2<T>(); }
template <typename T> static void Vector2_copyConstructor(const Vector2<T> &other, Vector2<T> *self) { new(self)Vector2<T>(other); }
template <typename T> static void Vector2_initConstructor(T x, T y, Vector2<T> *self)                { new(self)Vector2<T>(x, y); }
template <typename T> static void Vector2_listConstructor(T *list, Vector2<T> *self)                 { new(self)Vector2<T>(list[0], list[1]); }

//------------------------------------------------------------------------------
void register_Vector2i(asIScriptEngine & e)
{
    const char * t = "Vector2i";

    // Register type
    asCheck(e.RegisterObjectType(t, sizeof(Vector2i), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Fields
    asCheck(e.RegisterObjectProperty(t, "int x", asOFFSET(Vector2i, x)));
    asCheck(e.RegisterObjectProperty(t, "int y", asOFFSET(Vector2i, y)));

    // Constructors
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vector2_defaultConstructor<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const Vector2i &in)", asFUNCTION(Vector2_copyConstructor<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(Vector2_initConstructor<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {int, int}", asFUNCTION(Vector2_listConstructor<s32>), asCALL_CDECL_OBJLAST));

    // Register the operator overloads
    asCheck(e.RegisterObjectMethod(t, "Vector2i &opAddAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator+=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i &opSubAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator-=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i &opMulAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator*=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i &opDivAssign(const Vector2i &in)", asMETHODPR(Vector2i, operator/=, (const Vector2i &), Vector2i&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "bool opEquals(const Vector2i &in) const", asMETHODPR(Vector2i, operator==, (const Vector2i &) const, bool), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i opAdd(const Vector2i &in) const", asMETHODPR(Vector2i, operator+, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i opSub(const Vector2i &in) const", asMETHODPR(Vector2i, operator-, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i opMul(const Vector2i &in) const", asMETHODPR(Vector2i, operator*, (const Vector2i &) const, Vector2i), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Vector2i opDiv(const Vector2i &in) const", asMETHODPR(Vector2i, operator/, (const Vector2i &) const, Vector2i), asCALL_THISCALL));

    // Methods
    asCheck(e.RegisterObjectMethod(t, "float distanceTo(const Vector2i &in) const", asMETHOD(Vector2i, getDistanceFrom), asCALL_THISCALL));

    // Properties
    asCheck(e.RegisterObjectMethod(t, "float get_length() const", asMETHOD(Vector2i, length), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "float set_length(float l) const", asMETHOD(Vector2i, setLength), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "int get_lengthSQ() const", asMETHOD(Vector2i, getLengthSQ), asCALL_THISCALL));

}

} // namespace sn


