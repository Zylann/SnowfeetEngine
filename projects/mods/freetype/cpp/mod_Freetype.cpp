#include "mod_Freetype.hpp"
#include "FontLoader.hpp"

namespace freetype
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<FontLoader>();
    }
}

int loadSnowfeetModule_ModFreetype(ModuleLoadArgs args)
{
	freetype::registerObjectTypes(*(args.objectTypeDatabase));
    return 0;
}

int unloadSnowfeetModule_ModFreetype(ModuleUnloadArgs args)
{
    return 0;
}

