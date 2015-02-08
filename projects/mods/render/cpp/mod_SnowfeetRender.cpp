#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetRender.h"
#include "ShaderProgram.hpp"
#include "sq_binding/sq_render.hpp"

#include "entities/RenderManager.hpp"
#include "entities/Camera.hpp"
#include "entities/Drawable.hpp"


namespace sn {
namespace render {

    void registerObjectTypes(ObjectTypeDatabase & otb)
    {
        otb.registerType<RenderManager>();
        otb.registerType<Camera>();
        otb.registerType<Drawable>();
    }

} // namespace render
} // namespace sn

int loadSnowfeetModule_ModRender(ModuleLoadArgs args)
{
    sn::render::registerObjectTypes(*(args.objectTypeDatabase));

    sn::render::bindRenderManager(args.squirrelVM);

    return 0;
}

int unloadSnowfeetModule_ModRender(ModuleUnloadArgs args)
{
    return 0;
}


