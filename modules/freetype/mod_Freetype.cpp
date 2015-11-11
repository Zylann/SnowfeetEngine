#include "mod_Freetype.hpp"
#include "FontLoader.hpp"
#include "Font.hpp"

int loadSnowfeetModule_ModFreetype(ModuleLoadArgs args)
{
    using namespace sn;

    ObjectTypeDatabase & otb = *args.objectTypeDatabase;

    otb.registerType<FontLoader>();
    otb.registerType<Font>();

    return 0;
}

int unloadSnowfeetModule_ModFreetype(ModuleUnloadArgs args)
{
    return 0;
}

