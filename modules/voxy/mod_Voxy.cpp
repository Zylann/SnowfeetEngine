#include "mod_Voxy.h"


namespace sn
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        //otb.registerType<MyClass>();
        // TODO Register types
    }
}

int loadSnowfeetModule_ModVoxy(ModuleLoadArgs args)
{
    sn::registerObjectTypes(*(args.objectTypeDatabase));
    // TODO implement
    return 0;}

int unloadSnowfeetModule_ModVoxy(ModuleUnloadArgs args)
{
    // TODO implement
    return 0;
}

