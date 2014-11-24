#include <angelscript.h>
#include "mod_SnowfeetRender.h"
#include "Shader.hpp"

#include <core/asset/AssetDatabase.hpp>

int registerSnowfeetModule(asIScriptEngine * engine)
{
    //adb.addLoader<sn::Shader>(new sn::ShaderLoader());
    return 0;
}

int unloadSnowfeetModule(asIScriptEngine * engine)
{
    return 0;
}


