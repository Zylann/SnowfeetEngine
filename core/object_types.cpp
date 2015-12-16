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

#include "util/macros.h"

namespace sn
{

void registerObjectTypes(ObjectTypeDatabase & otb)
{
    // Base
    otb.registerType<Object>                            (SN_TYPESTRING(sn::Object));
    otb.registerType<ScriptableObject, Object>          (SN_TYPESTRING(sn::ScriptableObject));
    otb.registerType<IDriver, Object>                   (SN_TYPESTRING(sn::IDriver));

    // Entities
    otb.registerType<Entity, ScriptableObject>          (SN_TYPESTRING(sn::Entity));
    otb.registerType<Entity3D, Entity>                  (SN_TYPESTRING(sn::Entity3D));
    otb.registerType<Scene, Entity>                     (SN_TYPESTRING(sn::Scene));
    otb.registerType<Rotate, Entity>                    (SN_TYPESTRING(sn::Rotate));
    otb.registerType<KeyboardMove, Entity>              (SN_TYPESTRING(sn::KeyboardMove));

    // Assets base
    otb.registerType<Asset, ScriptableObject>           (SN_TYPESTRING(sn::Asset));
    otb.registerType<AssetLoader, Object>               (SN_TYPESTRING(sn::AssetLoader));
    otb.registerType<ObjectDB, Asset>                   (SN_TYPESTRING(sn::ObjectDB));
    otb.registerType<PackedEntity, ObjectDB>            (SN_TYPESTRING(sn::PackedEntity));
    otb.registerType<PackedEntityLoader, AssetLoader>   (SN_TYPESTRING(sn::PackedEntityLoader));

}

} // namespace sn

