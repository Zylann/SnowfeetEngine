#include <core/app/ModuleArgs.h>
#include <core/asset/AssetDatabase.h>

#include "mod_SnowfeetImage.h"

#include "Image.h"
#include "ImageLoader.hpp"

#include <core/util/macros.h>


int loadSnowfeetModule_ModImage(ModuleLoadArgs args)
{
    auto & otb = *args.objectTypeDatabase;

    otb.registerType<sn::Image, sn::Asset>(SN_TYPESTRING(sn::Image));
    otb.registerType<sn::ImageLoader, sn::AssetLoader>(SN_TYPESTRING(sn::ImageLoader));

    return 0;
}

int unloadSnowfeetModule_ModImage(ModuleUnloadArgs args)
{
    return 0;
}


