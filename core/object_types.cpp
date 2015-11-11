//
// object_types.cpp
// Copyright (C) 2015-2015 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#include "object_types.h"

#include "scene/Entity.h"
#include "scene/Entity3D.h"
#include "scene/Scene.h"
#include "scene/helpers/Rotate.h"
#include "scene/helpers/KeyboardMove.h"
#include "scene/PackedEntity.h"
#include "scene/PackedEntityLoader.h"

#include "asset/Asset.h"
#include "asset/AssetLoader.h"

#include "app/Driver.h"

namespace sn
{

void registerObjectTypes(ObjectTypeDatabase & otb)
{
    // Base
    otb.registerType<Object>();
    otb.registerType<ScriptableObject>();

    // Drivers
    otb.registerType<IDriver>();

    // Entities
    otb.registerType<Entity>();
    otb.registerType<Entity3D>();
    otb.registerType<Scene>();
    otb.registerType<Rotate>();
    otb.registerType<KeyboardMove>();

    // Assets base
    otb.registerType<Asset>();
    otb.registerType<AssetLoader>();
    otb.registerType<ObjectDB>();
    otb.registerType<PackedEntity>();
    otb.registerType<PackedEntityLoader>();

}

} // namespace sn

