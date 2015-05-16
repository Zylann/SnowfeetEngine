#include "mod_Assimp.h"


namespace assimp_mod
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        //otb.registerType<MyClass>();
        // TODO Register types
    }
}

int loadSnowfeetModule_ModAssimp(ModuleLoadArgs args)
{
	assimp_mod::registerObjectTypes(*(args.objectTypeDatabase));
	// TODO implement
    return 0;
}

int unloadSnowfeetModule_ModAssimp(ModuleUnloadArgs args)
{
    return 0;
}

