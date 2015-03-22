#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetRender.h"

#include "Mesh.hpp"
#include "Material.hpp"
#include "ShaderProgram.hpp"

#include "entities/RenderManager.hpp"
#include "entities/Camera.hpp"
#include "entities/Drawable.hpp"

#include "sq_binding/sq_render.hpp"


namespace sn {
namespace render {

    void registerObjectTypes(ObjectTypeDatabase & otb)
    {
        // Entities
        otb.registerType<RenderManager>();
        otb.registerType<Camera>();
        otb.registerType<Drawable>();

        // Assets
        otb.registerType<Mesh>();
        otb.registerType<Material>();
        otb.registerType<ShaderProgram>();
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


