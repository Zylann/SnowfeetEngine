#include <core/app/ModuleArgs.h>
#include <core/asset/AssetDatabase.h>

#include "mod_SnowfeetImage.h"

#include "Image.h"
#include "ImageLoader.hpp"


int loadSnowfeetModule_ModImage(ModuleLoadArgs args)
{
    auto & otb = *args.objectTypeDatabase;

    otb.registerType<sn::Image>();
    otb.registerType<sn::ImageLoader>();

    return 0;
}

int unloadSnowfeetModule_ModImage(ModuleUnloadArgs args)
{
    return 0;
}


