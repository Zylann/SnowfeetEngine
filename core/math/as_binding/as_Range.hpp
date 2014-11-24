#ifndef __HEADER_SN_AS_RANGE__
#define __HEADER_SN_AS_RANGE__

#include <core/app/ScriptEngine.hpp>
#include <core/math/Range.hpp>

namespace sn
{

//------------------------------------------------------------------------------
template <typename T> void Range_initConstructor(T min, T max, Range<T> *self)           { new(self)Range<T>(min, max); }
template <typename T> void Range_copyConstructor(const Range<T> &other, Range<T> *self)  { new(self)Range<T>(other); }
template <typename T> void Range_listConstructor(T *list, Range<T> *self)                { new(self)Range<T>(list[0], list[1]); }

//------------------------------------------------------------------------------
template <typename T>
void register_Range(asIScriptEngine & e, const std::string & c, const std::string & t)
{
    // Register type
    asCheck(e.RegisterObjectType(c.c_str(), sizeof(Range<T>), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Constructors
    asCheck(e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_CONSTRUCT, ("void f(const " + c + " &in)").c_str(), asFUNCTION(Range_copyConstructor<T>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_CONSTRUCT, ("void f(" + t + "," + t + ")").c_str(), asFUNCTION(Range_initConstructor<T>), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_LIST_CONSTRUCT, ("void f(const int &in) {" + t + "," + t + "}").c_str(), asFUNCTION(Range_listConstructor<T>), asCALL_CDECL_OBJLAST));

    // Properties
    asCheck(e.RegisterObjectMethod(c.c_str(), (t + " get_min()").c_str(), asMETHOD(Range<T>, min), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (t + " get_max()").c_str(), asMETHOD(Range<T>, max), asCALL_THISCALL));

    // Methods
    asCheck(e.RegisterObjectMethod(c.c_str(), (t + " set(" + t + " min," + t + " max)").c_str(), asMETHOD(Range<T>, set), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), ("bool contains(" + t + " x)").c_str(), asMETHOD(Range<T>, contains), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), "float lerp(float t)", asMETHOD(Range<T>, lerp), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), ("float inverseLerp(" + t + " x)").c_str(), asMETHOD(Range<T>, inverseLerp), asCALL_THISCALL));

}

} // namespace sn

#endif // __HEADER_SN_AS_RANGE__

