#include <core/app/ModuleArgs.hpp>
#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetImage.h"

#include "ImageLoader.hpp"


int loadSnowfeetModule_ModImage(ModuleLoadArgs args)
{
    auto & otb = *args.objectTypeDatabase;

    otb.registerType<sn::ImageLoader>();

    return 0;
}

int unloadSnowfeetModule_ModImage(ModuleUnloadArgs args)
{
    return 0;
}


