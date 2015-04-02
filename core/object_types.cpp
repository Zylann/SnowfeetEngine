//
// object_types.cpp
// Copyright (C) 2015-2015 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#include "object_types.hpp"

#include "scene/Entity.hpp"
#include "scene/Entity3D.hpp"
#include "scene/Scene.hpp"
#include "scene/VRHeadset.hpp"
#include "scene/helpers/Rotate.hpp"
#include "scene/helpers/KeyboardMove.hpp"

#include "asset/Asset.hpp"
#include "asset/AssetLoader.hpp"
#include "asset/base/Mesh.hpp"
#include "asset/base/Material.hpp"

namespace sn
{

void registerObjectTypes(ObjectTypeDatabase & otb)
{
    // Base
    otb.registerType<Object>();
    otb.registerType<ScriptObject>();

    // Entities
    otb.registerType<Entity>();
    otb.registerType<Entity3D>();
    otb.registerType<Scene>();
    otb.registerType<VRHeadset>();
    otb.registerType<Rotate>();
    otb.registerType<KeyboardMove>();

    // Assets base
    otb.registerType<Asset>();
    otb.registerType<AssetLoader>();
    otb.registerType<Mesh>();
    otb.registerType<Material>();

}

} // namespace sn

