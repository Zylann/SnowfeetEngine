#ifndef __HEADER_SN_SNOWFEET_IMAGE__
#define __HEADER_SN_SNOWFEET_IMAGE__

#include <core/app/ModuleArgs.hpp>
#include <core/config.hpp>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModImage(ModuleLoadArgs * engine);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModImage(ModuleLoadArgs * engine);
}

#endif // __HEADER_SN_SNOWFEET_IMAGE__

