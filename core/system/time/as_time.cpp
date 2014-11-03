#include "as_time.hpp"
#include "../../util/Log.hpp"
#include "../../app/ScriptEngine.hpp"

#include "Time.hpp"
#include "Clock.hpp"

#include <new>

namespace sn
{

//------------------------------------------------------------------------------
void Clock_constructor(void *memory)
{
    // Initialize the pre-allocated memory by calling the
    // object constructor with the placement-new operator
    new(memory)Clock();
}

//------------------------------------------------------------------------------
//void Clock_destructor(void *memory)
//{
//	// Uninitialize the memory by calling the object destructor
//	((Clock*)memory)->~Clock();
//}

//------------------------------------------------------------------------------
void register_Time(asIScriptEngine * engine)
{
    asCheck(engine->RegisterObjectType("Time", sizeof(Time), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    asCheck(engine->RegisterObjectMethod("Time", "float asSeconds() const", asMETHOD(Time, asSeconds), asCALL_THISCALL));
    asCheck(engine->RegisterObjectMethod("Time", "int asMilliseconds() const", asMETHOD(Time, asMilliseconds), asCALL_THISCALL));
    asCheck(engine->RegisterObjectMethod("Time", "int64 asMicroseconds() const", asMETHOD(Time, asMicroseconds), asCALL_THISCALL));
}

//------------------------------------------------------------------------------
void register_Clock(asIScriptEngine * engine)
{
    asCheck(engine->RegisterObjectType("Clock", sizeof(Clock), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    asCheck(engine->RegisterObjectBehaviour("Clock", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Clock_constructor), asCALL_CDECL_OBJLAST));
    //asCheck(engine->RegisterObjectBehaviour("Clock", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Clock_constructor), asCALL_CDECL_OBJLAST));

    asCheck(engine->RegisterObjectMethod("Clock", "Time restart()", asMETHOD(Clock, restart), asCALL_THISCALL));
    asCheck(engine->RegisterObjectMethod("Clock", "Time getElapsedTime() const", asMETHOD(Clock, getElapsedTime), asCALL_THISCALL));
}

} // namespace sn

