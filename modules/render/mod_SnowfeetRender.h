#ifndef __HEADER_SN_SNOWFEET_RENDER__
#define __HEADER_SN_SNOWFEET_RENDER__

#include <core/app/ModuleArgs.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModRender(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModRender(ModuleUnloadArgs args);
}

#endif // __HEADER_SN_SNOWFEET_RENDER__

