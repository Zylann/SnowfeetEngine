#include "mod_Freetype.hpp"
#include "FontLoader.hpp"
#include "Font.hpp"

namespace freetype
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<sn::Font>();
        otb.registerType<FontLoader>();
        otb.registerType<Font>();
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

