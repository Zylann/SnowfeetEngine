//
// object_types.cpp
// Copyright (C) 2015-2015 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#include "object_types.h"

#include "scene/Entity.h"
#include "scene/Entity3D.h"
#include "scene/Scene.h"
#include "scene/VRHeadset.h"
#include "scene/helpers/Rotate.h"
#include "scene/helpers/KeyboardMove.h"
#include "scene/Drawable.h"
#include "scene/PackedEntity.h"
#include "scene/PackedEntityLoader.h"

#include "asset/Asset.h"
#include "asset/AssetLoader.h"
#include "asset/base/Mesh.h"
#include "asset/base/Material.h"
#include "asset/base/Image.h"
#include "asset/base/TextureBase.h"
#include "asset/base/Font.h"

#include "drivers/Driver.h"
#include "drivers/VideoDriver.h"

namespace sn
{

void registerObjectTypes(ObjectTypeDatabase & otb)
{
    // Base
    otb.registerType<Object>();
    otb.registerType<ScriptableObject>();

    // Drivers
    otb.registerType<IDriver>();
    otb.registerType<IVideoDriver>();

    // Entities
    otb.registerType<Entity>();
    otb.registerType<Entity3D>();
    otb.registerType<Scene>();
    otb.registerType<VRHeadset>();
    otb.registerType<Rotate>();
    otb.registerType<KeyboardMove>();
    otb.registerType<Drawable>("DrawableBase"); // TODO Drawable shouldn't be there...

    // Assets base
    otb.registerType<Asset>();
    otb.registerType<AssetLoader>();
    otb.registerType<ObjectDB>();
    otb.registerType<Mesh>();
    otb.registerType<Material>();
    otb.registerType<Image>();
    otb.registerType<TextureBase>();
    otb.registerType<Font>();
    otb.registerType<PackedEntity>();
    otb.registerType<PackedEntityLoader>();

}

} // namespace sn

