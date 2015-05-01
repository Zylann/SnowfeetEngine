#include "mod_Freetype.hpp"


namespace freetype
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        //otb.registerType<MyClass>();
        // TODO Register types
    }
}

int loadSnowfeetModule_ModFreetype(ModuleLoadArgs args)
{
	freetype::registerObjectTypes(*(args.objectTypeDatabase));
	// TODO implement
}

int unloadSnowfeetModule_ModFreetype(ModuleUnloadArgs args)
{
	// TODO implement
}

