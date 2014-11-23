#include <angelscript.h>
#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetImage.hpp"
#include "as_binding/as_image.hpp"


void registerSnowfeetModule(asIScriptEngine * engine)
{
    sn::register_image(*engine);
    //adb.addLoader<sn::Shader>(new sn::ShaderLoader());
}

namespace sn
{

} // namespace sn


