#include "mod_Oculus.hpp"

#include "HeadTracker.hpp"

namespace sn {
namespace oculus
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<HeadTracker>();
    }

} // namespace oculus
} // namespace sn

int loadSnowfeetModule_ModOculus(ModuleLoadArgs args)
{
	sn::oculus::registerObjectTypes(*(args.objectTypeDatabase));
    return 0;
}

int unloadSnowfeetModule_ModOculus(ModuleUnloadArgs args)
{
    return 0;
}

