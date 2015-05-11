#include <core/asset/AssetDatabase.hpp>

#include "mod_SnowfeetRender.h"

#include "Material.hpp"
#include "ShaderProgram.hpp"

#include "entities/RenderManager.hpp"
#include "entities/Camera.hpp"
#include "entities/Drawable.hpp"

#include "loaders/BasicMeshLoader.hpp"
#include "loaders/MaterialLoader.hpp"
#include "loaders/TextureLoader.hpp"

#include "VideoDriver.hpp"

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
        otb.registerType<Material>();
        otb.registerType<ShaderProgram>();
        otb.registerType<Texture>();
        otb.registerType<RenderTexture>();

        // Loaders
        otb.registerType<BasicMeshLoader>();
        otb.registerType<MaterialLoader>();
        otb.registerType<TextureLoader>();

        otb.registerType<VideoDriver>();
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


