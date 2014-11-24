#ifndef __HEADER_SN_SNOWFEET_IMAGE__
#define __HEADER_SN_SNOWFEET_IMAGE__

#include <angelscript.h>
#include <core/config.hpp>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule(asIScriptEngine * engine);
    SN_MOD_EXPORT int unloadSnowfeetModule(asIScriptEngine * engine);
}

#endif // __HEADER_SN_SNOWFEET_IMAGE__

