#include <core/app/ModuleArgs.hpp>
#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetImage.h"


int loadSnowfeetModule_ModImage(ModuleLoadArgs args)
{
    //sn::register_image(*args.scriptEngine);
    //adb.addLoader<sn::Shader>(new sn::ShaderLoader());
    return 0;
}

int unloadSnowfeetModule_ModImage(ModuleUnloadArgs args)
{
    return 0;
}


