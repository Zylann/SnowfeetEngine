#ifndef __HEADER_SN_SAMPLE_MOD_NATIVEHELLO__
#define __HEADER_SN_SAMPLE_MOD_NATIVEHELLO__

#include <core/app/ModuleArgs.hpp>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_SampleNativeHello(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_SampleNativeHello(ModuleUnloadArgs args);
}

#endif // __HEADER_SN_SAMPLE_MOD_NATIVEHELLO__

