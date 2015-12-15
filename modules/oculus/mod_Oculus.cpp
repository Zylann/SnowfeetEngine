#include "mod_Oculus.hpp"

#include "HeadTracker.hpp"

#include <core/util/macros.h>

namespace
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<sn::oculus::HeadTracker, sn::VRHeadset>(SN_TYPESTRING(sn::oculus::HeadTracker));
    }
}

int loadSnowfeetModule_ModOculus(ModuleLoadArgs args)
{
	registerObjectTypes(*(args.objectTypeDatabase));
    return 0;
}

int unloadSnowfeetModule_ModOculus(ModuleUnloadArgs args)
{
    return 0;
}

