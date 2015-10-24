#ifndef __HEADER_MOD_SN_VOXY__
#define __HEADER_MOD_SN_VOXY__

#include <core/app/ModuleArgs.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModVoxy(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModVoxy(ModuleUnloadArgs args);
}

#endif // __HEADER_MOD_SN_VOXY__

