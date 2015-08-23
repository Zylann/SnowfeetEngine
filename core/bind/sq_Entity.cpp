/*
sq_Entity.cpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../scene/Entity.h"
#include "../scene/Scene.h"
#include "sq_core.h"
#include <iostream>
#include "../squirrel/bind_macros.h"
		
using namespace squirrel;

namespace sn
{
	namespace 
	{
		CURRENT_CLASS(Entity)

		BEGIN_METHOD(getName)
			sq_pushstring(vm, self->getName().c_str(), -1);
			return 1;
		END_METHOD

		BEGIN_METHOD(setName)
			const char * str = getString(vm, 2);
			self->setName(str ? str : "");
			return 0;
		END_METHOD

		BEGIN_METHOD(getScene)
			auto * p = self->getScene();
			if (p)
				p->pushScriptObject(vm);
			else
				sq_pushnull(vm);
			return 1;
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
                    // TODO Have a helper for this that doesn't looks too macro-ish
                    std::stringstream ss;
                    ss << "Invalid child index (" << i << ")";
                    std::string s = ss.str();
					return sq_throwerror(vm, s.c_str());
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
                std::stringstream ss;
                ss << "Invalid argument type to " << SN_FUNCNAME;
                std::string s = ss.str();
                return sq_throwerror(vm, s.c_str());
			}
        END_METHOD

        BEGIN_METHOD(setUpdatable)
            bool enable = getBool(vm, 2);
            self->setUpdatable(enable);
            return 0;
        END_METHOD

        BEGIN_METHOD(getScript)
            squirrel::Instance & obj = self->getScript();
            if (obj.isNull() || obj.getVM() != vm)
                sq_pushnull(vm);
            else
                sq_pushobject(vm, obj.getObject());
            return 1;
        END_METHOD

		SQInteger createChild(HSQUIRRELVM vm)
		{
			GET_SELF();
			SQInteger argType = sq_gettype(vm, 2);
			ScriptableObject * child = nullptr;
			if (argType == OT_STRING)
			{
				const SQChar * typeName = nullptr;
				child = self->createChild(typeName);
			}
			else
			{
				child = self->createChild();
			}
			if (child)
				child->pushScriptObject(vm);
			else
				sq_pushnull(vm);
			return 1;
		}

	}

void bindEntity(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Entity>(vm)
        .setMethod("getScene", getScene)
        .setMethod("getParent", getParent)
        .setMethod("getChild", getChild)
        .setMethod("getChildCount", getChild)
        .setMethod("getName", getName)
        .setMethod("setName", setName, 1, "s")
		.setMethod("isEnabled", isEnabled)
		.setMethod("setEnabled", setEnabled, 1, "b")
		.setMethod("hasTag", hasTag)
		.setMethod("addTag", addTag, 1, "s")
		.setMethod("removeTag", removeTag)
        .setMethod("getScript", getScript)
        .setMethod("setUpdatable", setUpdatable, -1, "b")
		.setMethod("createChild", createChild, -1, "s")
	;
}

} // namespace sn


