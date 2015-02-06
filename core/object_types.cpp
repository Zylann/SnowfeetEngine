//
// object_types.cpp
// Copyright (C) 2015-2015 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#include "object_types.hpp"

#include "scene/Entity.hpp"
#include "scene/Scene.hpp"

namespace sn
{

void registerObjectTypes(ObjectTypeDatabase & otb)
{
    otb.registerType<ScriptObject>();
    otb.registerType<Entity>();
    otb.registerType<Scene>();
}

} // namespace sn

