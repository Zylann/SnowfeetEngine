#include <angelscript.h>
#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetImage.h"
#include "as_binding/as_image.hpp"


int registerSnowfeetModule(asIScriptEngine * engine)
{
    sn::register_image(*engine);
    //adb.addLoader<sn::Shader>(new sn::ShaderLoader());
    return 0;
}

int unloadSnowfeetModule(asIScriptEngine * engine)
{
    return 0;
}


