#include "mod_Freetype.hpp"
#include "FontLoader.hpp"
#include "Font.hpp"

#include <core/util/macros.h>

int loadSnowfeetModule_ModFreetype(ModuleLoadArgs args)
{
    using namespace sn;

    ObjectTypeDatabase & otb = *args.objectTypeDatabase;

    otb.registerType<FontLoader, AssetLoader>(SN_TYPESTRING(sn::FontLoader));
    otb.registerType<Font, Asset>(SN_TYPESTRING(sn::Font));

    return 0;
}

int unloadSnowfeetModule_ModFreetype(ModuleUnloadArgs args)
{
    return 0;
}

