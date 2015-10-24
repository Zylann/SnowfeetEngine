#ifndef __HEADER_MOD_ASSIMP_MOD_ASSIMP__
#define __HEADER_MOD_ASSIMP_MOD_ASSIMP__

#include <core/app/ModuleArgs.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModAssimp(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModAssimp(ModuleUnloadArgs args);
}

#endif // __HEADER_MOD_ASSIMP_MOD_ASSIMP__

