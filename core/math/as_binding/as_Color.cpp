#include "as_math.hpp"
#include "../Color.hpp"
#include "../../app/ScriptEngine.hpp"

namespace sn
{

//------------------------------------------------------------------------------
static void Color_defaultConstructor(Color *self)                      { new(self)Color(); }
static void Color_copyConstructor(const Color &other, Color *self)     { new(self)Color(other); }
static void Color_initConstructor(u8 r, u8 g, u8 b, u8 a, Color *self) { new(self)Color(r, g, b, a); }
static void Color_listConstructor(u8 *list, Color *self)               { new(self)Color(list[0], list[1], list[2], list[3]); }

//------------------------------------------------------------------------------
void register_Color(asIScriptEngine & e)
{
    const char * t = "Color";

    // Register type
    asCheck(e.RegisterObjectType(t, sizeof(Color), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Fields
    asCheck(e.RegisterObjectProperty(t, "uint8 r", asOFFSET(Color, r)));
    asCheck(e.RegisterObjectProperty(t, "uint8 g", asOFFSET(Color, g)));
    asCheck(e.RegisterObjectProperty(t, "uint8 b", asOFFSET(Color, b)));
    asCheck(e.RegisterObjectProperty(t, "uint8 a", asOFFSET(Color, a)));

    // Constructors
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Color_defaultConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const Color &in)", asFUNCTION(Color_copyConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(uint8, uint8, uint8, uint8)", asFUNCTION(Color_initConstructor), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {uint8, uint8, uint8, uint8}", asFUNCTION(Color_listConstructor), asCALL_CDECL_OBJLAST));

    // Operator overloads
    // TODO Color: * and / operators for AS binding
    asCheck(e.RegisterObjectMethod(t, "Color &opAddAssign(const Color &in)", asMETHODPR(Color, operator+=, (const Color &), Color&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Color &opSubAssign(const Color &in)", asMETHODPR(Color, operator-=, (const Color &), Color&), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "Color &opMulAssign(const Color &in)", asMETHODPR(Color, operator*=, (const Color &), Color&), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "Color &opDivAssign(const Color &in)", asMETHODPR(Color, operator/=, (const Color &), Color&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "bool opEquals(const Color &in) const", asMETHODPR(Color, operator==, (const Color &) const, bool), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Color opAdd(const Color &in) const", asMETHODPR(Color, operator+, (const Color &) const, Color), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t, "Color opSub(const Color &in) const", asMETHODPR(Color, operator-, (const Color &) const, Color), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "Color opMul(const Color &in) const", asMETHODPR(Color, operator*, (const Color &) const, Color), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "Color opDiv(const Color &in) const", asMETHODPR(Color, operator/, (const Color &) const, Color), asCALL_THISCALL));

    // Methods
    asCheck(e.RegisterObjectMethod(t, "void setf(float, float, float, float)", asMETHODPR(Color, setf, (f32, f32, f32, f32), void), asCALL_THISCALL));
}

} // namespace sn


