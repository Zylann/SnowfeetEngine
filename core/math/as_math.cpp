#include "as_math.hpp"
#include "../app/ScriptEngine.hpp"
#include "math.hpp"
#include "Vector2.hpp"
#include "Color.hpp"

namespace sn
{

//------------------------------------------------------------------------------
f32 asRand() { return math::randf(); }
f32 asRandMinMax(f32 min, f32 max) { return math::randf(min, max); }
s32 asRandIntMinMax(s32 min, s32 max) { return math::rand(min, max); }

//------------------------------------------------------------------------------
void register_math(asIScriptEngine * e)
{
    asCheck(e->RegisterGlobalFunction("float rand()", asFUNCTIONPR(asRand, (), f32), asCALL_CDECL));
    asCheck(e->RegisterGlobalFunction("float rand(float, float)", asFUNCTIONPR(asRandMinMax, (f32, f32), f32), asCALL_CDECL));
    asCheck(e->RegisterGlobalFunction("int randint(int, int)", asFUNCTIONPR(asRandIntMinMax, (s32, s32), s32), asCALL_CDECL));

    register_Vector2i(e);
}

//==============================================================================
// Vector2i
//==============================================================================

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

//==============================================================================
// Color
//==============================================================================

//------------------------------------------------------------------------------
static void Color_defaultConstructor(Color *self)
{
    new(self)Color();
}

//------------------------------------------------------------------------------
static void Color_copyConstructor(const Color &other, Color *self)
{
    new(self)Color(other);
}

//------------------------------------------------------------------------------
static void Color_initConstructor(u8 r, u8 g, u8 b, u8 a, Color *self)
{
    new(self)Color(r, g, b, a);
}

//------------------------------------------------------------------------------
static void Color_listConstructor(u8 *list, Color *self)
{
    new(self)Color(list[0], list[1], list[2], list[3]);
}

//------------------------------------------------------------------------------
void register_Color(asIScriptEngine * e)
{
    const char * t = "Color";

    // Register type
    asCheck(e->RegisterObjectType(t, sizeof(Color), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Fields
    asCheck(e->RegisterObjectProperty(t, "uint8 r", asOFFSET(Color, r)));
    asCheck(e->RegisterObjectProperty(t, "uint8 g", asOFFSET(Color, g)));
    asCheck(e->RegisterObjectProperty(t, "uint8 b", asOFFSET(Color, b)));
    asCheck(e->RegisterObjectProperty(t, "uint8 a", asOFFSET(Color, a)));

    // Constructors
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Color_defaultConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const Color &in)", asFUNCTION(Color_copyConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(uint8, uint8, uint8, uint8)", asFUNCTION(Color_initConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e->RegisterObjectBehaviour(t, asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {uint8, uint8, uint8, uint8}", asFUNCTION(Color_listConstructor), asCALL_CDECL_OBJLAST));

    // Operator overloads
    // TODO Color: * and / operators for AS binding
    asCheck(e->RegisterObjectMethod(t, "Color &opAddAssign(const Color &in)", asMETHODPR(Color, operator+=, (const Color &), Color&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Color &opSubAssign(const Color &in)", asMETHODPR(Color, operator-=, (const Color &), Color&), asCALL_THISCALL));
    //asCheck(e->RegisterObjectMethod(t, "Color &opMulAssign(const Color &in)", asMETHODPR(Color, operator*=, (const Color &), Color&), asCALL_THISCALL));
    //asCheck(e->RegisterObjectMethod(t, "Color &opDivAssign(const Color &in)", asMETHODPR(Color, operator/=, (const Color &), Color&), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "bool opEquals(const Color &in) const", asMETHODPR(Color, operator==, (const Color &) const, bool), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Color opAdd(const Color &in) const", asMETHODPR(Color, operator+, (const Color &) const, Color), asCALL_THISCALL));
    asCheck(e->RegisterObjectMethod(t, "Color opSub(const Color &in) const", asMETHODPR(Color, operator-, (const Color &) const, Color), asCALL_THISCALL));
    //asCheck(e->RegisterObjectMethod(t, "Color opMul(const Color &in) const", asMETHODPR(Color, operator*, (const Color &) const, Color), asCALL_THISCALL));
    //asCheck(e->RegisterObjectMethod(t, "Color opDiv(const Color &in) const", asMETHODPR(Color, operator/, (const Color &) const, Color), asCALL_THISCALL));

    // Methods
    asCheck(e->RegisterObjectMethod(t, "void setf(float, float, float, float)", asMETHODPR(Color, setf,(f32,f32,f32,f32), void), asCALL_THISCALL));
}

} // namespace sn




