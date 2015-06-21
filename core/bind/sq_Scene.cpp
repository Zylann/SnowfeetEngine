/*
sq_Scene.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../scene/Scene.hpp"
#include "sq_core.h"
#include "../squirrel/bind_macros.h"

using namespace squirrel;

namespace sn
{
	namespace
	{
		CURRENT_CLASS(Scene)

		BEGIN_METHOD(getTaggedEntity)
			const char * tagName = getString(vm, 2);
			Entity * e = self->getTaggedEntity(tagName);
			if (e)
				e->pushScriptObject(vm);
			else
				sq_pushnull(vm);
			return 0;
		END_METHOD

		BEGIN_METHOD(getTaggedEntities)
			const char * tagName = getString(vm, 2);
			std::vector<Entity*> entities = self->getTaggedEntities(tagName);
			sq_newarray(vm, entities.size());
			for (u32 i = 0; i < entities.size(); ++i)
			{
				sq_pushinteger(vm, i);
				Entity * e = entities[i];
				e->pushScriptObject(vm);
				sq_set(vm, -3);
			}
			return 1;
	    END_METHOD

		BEGIN_METHOD(getDeltaTime)
			Time t = self->getDeltaTime();
			sq_pushfloat(vm, t.asSeconds());
			return 1;
		END_METHOD

		BEGIN_METHOD(getTimeSinceStartup)
			Time t = self->getTimeSinceStartup();
			sq_pushfloat(vm, t.asSeconds());
			return 1;
		END_METHOD

	}

void bindScene(HSQUIRRELVM vm)
{
	ScriptableObject::bindBase<Scene>(vm)
		.setMethod("getTaggedEntity", getTaggedEntity)
		.setMethod("getTaggedEntities", getTaggedEntities)
		.setMethod("getDeltaTime", getDeltaTime)
		.setMethod("getTimeSinceStartup", getTimeSinceStartup);
}

} // namespace sn

