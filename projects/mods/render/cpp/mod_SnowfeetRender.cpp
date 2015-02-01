#include "mod_SnowfeetRender.h"
#include "Shader.hpp"
#include "entities/RenderManager.hpp"
#include "sq_binding/sq_render.hpp"

#include <core/asset/AssetDatabase.hpp>

namespace sn {
namespace render {

    void registerObjectTypes(ObjectTypeDatabase & otb)
    {
        otb.registerType<RenderManager>();
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


