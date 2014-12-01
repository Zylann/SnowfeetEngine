#include <core/app/ModuleArgs.hpp>
#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetImage.h"
#include "as_binding/as_image.hpp"


int loadSnowfeetModule(ModuleLoadArgs args)
{
    sn::register_image(*args.scriptEngine);
    //adb.addLoader<sn::Shader>(new sn::ShaderLoader());
    return 0;
}

int unloadSnowfeetModule(ModuleUnloadArgs args)
{
    return 0;
}


