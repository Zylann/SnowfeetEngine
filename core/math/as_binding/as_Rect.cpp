#include "as_math.hpp"
#include "../Rect.hpp"
#include "../../app/ScriptEngine.hpp"

namespace sn
{

//------------------------------------------------------------------------------
template <typename T> static void Rect_defaultConstructor(Rect<T> *self) { new(self)Rect<T>(); }
template <typename T> static void Rect_copyConstructor(const Rect<T> &other, Rect<T> *self) { new(self)Rect<T>(other); }
template <typename T> static void Rect_initConstructor(s32 x, s32 y, u32 w, u32 h, Rect<T> *self) { new(self)Rect<T>(x, y, w, h); }

template <typename T> static u32 Rect_getWidth(Rect<T> * self) { return static_cast<u32>(self->width); }
template <typename T> static u32 Rect_getHeight(Rect<T> * self) { return static_cast<u32>(self->height); }

template <typename T> static s32 Rect_setWidth(Rect<T> * self, T w) { return self->width = w; }
template <typename T> static s32 Rect_setHeight(Rect<T> * self, T h) { return self->height = h; }

//------------------------------------------------------------------------------
void register_IntRect(asIScriptEngine & e)
{
    const char * t = "IntRect";

    // Register type
    asCheck(e.RegisterObjectType(t, sizeof(IntRect), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Constructors
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Rect_defaultConstructor<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const IntRect &in)", asFUNCTION(Rect_copyConstructor<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(int, int, uint, uint)", asFUNCTION(Rect_initConstructor<s32>), asCALL_CDECL_OBJLAST));

    // Fields
    asCheck(e.RegisterObjectProperty(t, "int x", asOFFSET(IntRect, x)));
    asCheck(e.RegisterObjectProperty(t, "int y", asOFFSET(IntRect, y)));
    asCheck(e.RegisterObjectMethod(t, "uint get_width()", asFUNCTION(Rect_getWidth<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "uint get_height()", asFUNCTION(Rect_getHeight<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "void set_width(uint x)", asFUNCTION(Rect_setWidth<s32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "void set_height(uint y)", asFUNCTION(Rect_setHeight<s32>), asCALL_CDECL_OBJLAST));

    // Methods
    asCheck(e.RegisterObjectMethod(t, "bool contains(int x, int y)", asMETHOD(IntRect, contains), asCALL_THISCALL));
}

//------------------------------------------------------------------------------
void register_FloatRect(asIScriptEngine & e)
{
    const char * t = "FloatRect";

    // Register type
    asCheck(e.RegisterObjectType(t, sizeof(FloatRect), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Constructors
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Rect_defaultConstructor<f32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(const FloatRect &in)", asFUNCTION(Rect_copyConstructor<f32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t, asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(Rect_initConstructor<f32>), asCALL_CDECL_OBJLAST));

    // Fields
    asCheck(e.RegisterObjectProperty(t, "float x", asOFFSET(FloatRect, x)));
    asCheck(e.RegisterObjectProperty(t, "float y", asOFFSET(FloatRect, y)));
    asCheck(e.RegisterObjectMethod(t, "float get_width()", asFUNCTION(Rect_getWidth<f32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "float get_height()", asFUNCTION(Rect_getHeight<f32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "void set_width(float x)", asFUNCTION(Rect_setWidth<f32>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t, "void set_height(float y)", asFUNCTION(Rect_setHeight<f32>), asCALL_CDECL_OBJLAST));

    // Methods
    asCheck(e.RegisterObjectMethod(t, "bool contains(float x, float y)", asMETHOD(FloatRect, contains), asCALL_THISCALL));
}

} // namespace sn

