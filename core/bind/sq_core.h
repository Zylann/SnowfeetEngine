/*
sq_Scene.h
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SQ_CORE__
#define __HEADER_SN_SQ_CORE__

#include <core/squirrel/bind_tools.h>
#include <core/scene/Entity3D.h>
#include <core/asset/Asset.h>

namespace sn
{

void bindVector3(HSQUIRRELVM vm);
void bindQuaternion(HSQUIRRELVM vm);

void bindJoystick(HSQUIRRELVM vm);

void bindEntity(HSQUIRRELVM vm);
void bindEntity3D(HSQUIRRELVM vm);
void bindScene(HSQUIRRELVM vm);

void bindAsset(HSQUIRRELVM vm);

/// \brief Binds all classes of the engine's core to a Squirrel VM.
void bindCore(HSQUIRRELVM vm);

} // namespace sn

namespace squirrel
{
    template <> inline SQUserPointer getTypeTag<sn::Vector3f>()    { return sn::bindVector3; }
    template <> inline SQUserPointer getTypeTag<sn::Quaternion>()  { return sn::bindQuaternion; }
    template <> inline SQUserPointer getTypeTag<sn::Entity>()      { return sn::bindEntity; }
    template <> inline SQUserPointer getTypeTag<sn::Entity3D>()    { return sn::bindEntity3D; }
    template <> inline SQUserPointer getTypeTag<sn::Scene>()       { return sn::bindScene; }
    template <> inline SQUserPointer getTypeTag<sn::Asset>()       { return sn::bindAsset; }
}

#endif // __HEADER_SN_SQ_CORE__


