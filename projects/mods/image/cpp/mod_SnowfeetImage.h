﻿#ifndef __HEADER_SN_SNOWFEET_IMAGE__
#define __HEADER_SN_SNOWFEET_IMAGE__

#include <core/app/ModuleArgs.h>
#include <core/config.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModImage(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModImage(ModuleUnloadArgs args);
}

#endif // __HEADER_SN_SNOWFEET_IMAGE__

