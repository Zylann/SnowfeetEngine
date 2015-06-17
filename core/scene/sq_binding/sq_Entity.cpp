/*
sq_Entity.cpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "sq_scene.hpp"
#include <iostream>
#include "../../squirrel/bind_macros.h"
		
namespace sn
{
	namespace 
	{
		BEGIN_METHOD(getName)
			sq_pushstring(vm, self->getName().c_str(), -1);
			return 1;
		END_METHOD

		BEGIN_METHOD(setName)
			const char * str = getString(vm, 2);
			self->setName(str ? str : "");
			return 0;
		END_METHOD

		BEGIN_METHOD(getParent)
			auto * p = self->getParent();
			if (p)
				p->pushScriptObject(vm);
			else
				sq_pushnull(vm);
			return 1;
		END_METHOD

		BEGIN_METHOD(isDestroyed)
			sq_pushbool(vm, self->getFlag(SN_EF_DESTROYED));
			return 1;
		END_METHOD

		BEGIN_METHOD(destroy)
			self->destroy();
			return 0;
		END_METHOD

		BEGIN_METHOD(isEnabled)
			sq_pushbool(vm, self->isEnabled());
			return 1;
		END_METHOD

		BEGIN_METHOD(setEnabled)
			self->setEnabled(getBool(vm, 2));
			return 0;
		END_METHOD

		BEGIN_METHOD(hasTag)
			sq_pushbool(vm, self->hasTag(getString(vm, 2)));
			return 1;
		END_METHOD

		BEGIN_METHOD(addTag)
			self->addTag(getString(vm, 2));
			return 0;
		END_METHOD

		BEGIN_METHOD(removeTag)
			self->removeTag(getString(vm, 2));
			return 0;
		END_METHOD

		BEGIN_METHOD(getChildCount)
			sq_pushinteger(vm, self->getChildCount());
			return 1;
		END_METHOD

		BEGIN_METHOD(getChild)
			SQObjectType t = sq_gettype(vm, 2);
			if (t == OT_INTEGER)
			{
				s32 i = getInt(vm, 2);
				if (i >= 0 && i < static_cast<s32>(self->getChildCount()))
				{
					Entity * child = self->getChildByIndex(i);
					child->pushScriptObject(vm);
					return 1;
				}
				else
				{
					SN_ERROR("Invalid child index (" << i << ")");
					return 0;
				}
			}
			else if (t == OT_STRING)
			{
				const char * str = getString(vm, 2);
				Entity * child = self->getChildByName(str);
				if (child)
					child->pushScriptObject(vm);
				else
					sq_pushnull(vm);
				return 1;
			}
			else
			{
				SN_ERROR("Invalid argument type to " << SN_FUNCNAME);
				return 0;
			}
		END_METHOD
	}

void registerEntity(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Entity>(vm)
        .setMethod("getParent", getParent)
        .setMethod("getChild", getChild)
        .setMethod("getChildCount", getChild)
        .setMethod("getName", getName)
        .setMethod("setName", setName, 1, "s")
		.setMethod("isEnabled", isEnabled)
		.setMethod("setEnabled", setEnabled, 1, "b")
		.setMethod("hasTag", hasTag)
		.setMethod("addTag", addTag, 1, "s")
		.setMethod("removeTag", removeTag);
}
    
} // namespace sn


