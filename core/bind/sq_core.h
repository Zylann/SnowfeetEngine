/*
sq_Scene.h
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SQ_CORE__
#define __HEADER_SN_SQ_CORE__

#include <squirrel.h>

namespace sn
{

void bindVector3(HSQUIRRELVM vm);
void bindQuaternion(HSQUIRRELVM vm);

void bindEntity(HSQUIRRELVM vm);
void bindEntity3D(HSQUIRRELVM vm);
void bindScene(HSQUIRRELVM vm);
void bindDrawable(HSQUIRRELVM vm);

void bindAsset(HSQUIRRELVM vm);
void bindMaterialBase(HSQUIRRELVM vm);

/// \brief Binds all classes of the engine's core to a Squirrel VM.
void bindCore(HSQUIRRELVM vm);

} // namespace sn

#endif // __HEADER_SN_SQ_CORE__


