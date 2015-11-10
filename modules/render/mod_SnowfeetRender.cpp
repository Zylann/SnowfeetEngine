#include <core/asset/AssetDatabase.h>

#include "mod_SnowfeetRender.h"

#include "Material.h"
#include "ShaderProgram.h"

#include "entities/RenderManager.h"
#include "entities/Camera.h"
#include "entities/MeshEntity.h"
#include "entities/VRHeadset.h"

#include "loaders/BasicMeshLoader.h"
#include "loaders/MaterialLoader.h"
#include "loaders/TextureLoader.h"
#include "loaders/RenderTextureLoader.h"
#include "loaders/ShaderLoader.h"

#include "VideoDriver.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include "bind/sq_render.h"


namespace sn {
namespace render {

    void registerObjectTypes(ObjectTypeDatabase & otb)
    {
        otb.registerType<sn::VRHeadset>();
        otb.registerType<sn::Mesh>();

        // Entities
        otb.registerType<RenderManager>();
        otb.registerType<Camera>();
        otb.registerType<Drawable>();
        otb.registerType<MeshEntity>();

        // Assets
        otb.registerType<Material>();
        otb.registerType<ShaderProgram>();
        otb.registerType<Texture>();
        otb.registerType<RenderTexture>();

        // Loaders
        otb.registerType<BasicMeshLoader>();
        otb.registerType<MaterialLoader>();
        otb.registerType<TextureLoader>();
        otb.registerType<RenderTextureLoader>();
        otb.registerType<ShaderLoader>();

        otb.registerType<VideoDriver>();
    }

} // namespace render
} // namespace sn

int loadSnowfeetModule_ModRender(ModuleLoadArgs args)
{
    sn::render::registerObjectTypes(*(args.objectTypeDatabase));
    sn::render::bindRenderModule(args.squirrelVM);
    return 0;
}

int unloadSnowfeetModule_ModRender(ModuleUnloadArgs args)
{
    return 0;
}


