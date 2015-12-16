#include <core/asset/AssetDatabase.h>

#include "mod_SnowfeetRender.h"

#include "Material.h"
#include "ShaderProgram.h"

#include "entities/RenderManager.h"
#include "entities/Camera.h"
#include "entities/MeshEntity.h"
#include "entities/VRHeadset.h"
#include "entities/CameraRenderStep.h"

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

#include <core/util/macros.h>

namespace
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        using namespace sn;

        // Entities
        otb.registerType<RenderManager, Entity>               (SN_TYPESTRING(sn::RenderManager));
        otb.registerType<Camera, Entity3D>                    (SN_TYPESTRING(sn::Camera));
        otb.registerType<Drawable, Entity3D>                  (SN_TYPESTRING(sn::Drawable));
        otb.registerType<MeshEntity, Drawable>                (SN_TYPESTRING(sn::MeshEntity));
        otb.registerType<RenderStep, Entity>                  (SN_TYPESTRING(sn::RenderStep));
        otb.registerType<CameraRenderStep, RenderStep>        (SN_TYPESTRING(sn::CameraRenderStep));
        otb.registerType<VRHeadset, Entity>                   (SN_TYPESTRING(sn::VRHeadset));

        // Assets
        otb.registerType<Mesh, Asset>                         (SN_TYPESTRING(sn::Mesh));
        otb.registerType<Material, Asset>                     (SN_TYPESTRING(sn::Material));
        otb.registerType<ShaderProgram, Asset>                (SN_TYPESTRING(sn::ShaderProgram));
        otb.registerType<Texture, Asset>                      (SN_TYPESTRING(sn::Texture));
        otb.registerType<RenderTexture, Asset>                (SN_TYPESTRING(sn::RenderTexture));

        // Loaders
        otb.registerType<BasicMeshLoader, AssetLoader>        (SN_TYPESTRING(sn::BasicMeshLoader));
        otb.registerType<MaterialLoader, AssetLoader>         (SN_TYPESTRING(sn::MaterialLoader));
        otb.registerType<TextureLoader, AssetLoader>          (SN_TYPESTRING(sn::TextureLoader));
        otb.registerType<RenderTextureLoader, AssetLoader>    (SN_TYPESTRING(sn::RenderTextureLoader));
        otb.registerType<ShaderLoader, AssetLoader>           (SN_TYPESTRING(sn::ShaderLoader));

        otb.registerType<VideoDriver, IDriver>                (SN_TYPESTRING(sn::VideoDriver));
    }
}

int loadSnowfeetModule_ModRender(ModuleLoadArgs args)
{
    registerObjectTypes(*(args.objectTypeDatabase));
    sn::bindRenderModule(args.squirrelVM);
    return 0;
}

int unloadSnowfeetModule_ModRender(ModuleUnloadArgs args)
{
    return 0;
}


