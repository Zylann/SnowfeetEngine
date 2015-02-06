#include "mod_NativeHello.hpp"
#include "NativeHello.hpp"

namespace hello
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<NativeHello>();
    }
}

int loadSnowfeetModule_SampleNativeHello(ModuleLoadArgs args)
{
    hello::registerObjectTypes(*args.objectTypeDatabase);
    return 0;
}

int unloadSnowfeetModule_SampleNativeHello(ModuleUnloadArgs args)
{
    return 0;
}

