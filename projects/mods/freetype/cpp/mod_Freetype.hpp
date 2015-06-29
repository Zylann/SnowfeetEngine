#ifndef __HEADER_MOD_FREETYPE__
#define __HEADER_MOD_FREETYPE__

#include <core/app/ModuleArgs.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModFreetype(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModFreetype(ModuleUnloadArgs args);
}

#endif // __HEADER_MOD_FREETYPE__

