/*
sq_Entity.cpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../scene/Entity.h"
#include "../scene/Scene.h"
#include "../asset/AssetDatabase.h"
#include "../scene/PackedEntity.h"
#include "sq_core.h"
#include <iostream>
#include "../squirrel/bind_macros.h"
		
using namespace squirrel;

namespace sn
{
	namespace 
	{
		CURRENT_CLASS(Entity)
        
        SQRESULT getName(HSQUIRRELVM vm)
        {
            GET_SELF();
			sq_pushstring(vm, self->getName().c_str(), -1);
			return 1;
        }

        SQRESULT setName(HSQUIRRELVM vm)
        {
            GET_SELF();
            const char * str = getString(vm, 2);
            self->setName(str ? str : "");
            return 0;
        }

        SQRESULT getScene(HSQUIRRELVM vm)
        {
            GET_SELF();
            auto * p = self->getScene();
            if (p)
                p->pushScriptObject(vm);
            else
                sq_pushnull(vm);
            return 1;
        }

        SQRESULT getParent(HSQUIRRELVM vm)
        {
            GET_SELF();
            auto * p = self->getParent();
            if (p)
                p->pushScriptObject(vm);
            else
                sq_pushnull(vm);
            return 1;
        }

        SQRESULT isDestroyed(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushbool(vm, self->getFlag(SN_EF_DESTROYED));
            return 1;
        }

        SQRESULT destroy(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->destroy();
            return 0;
        }

        SQRESULT isEnabled(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushbool(vm, self->isEnabled());
            return 1;
        }

        SQRESULT setEnabled(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->setEnabled(getBool(vm, 2));
            return 0;
        }

        SQRESULT hasTag(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushbool(vm, self->hasTag(getString(vm, 2)));
            return 1;
        }

        SQRESULT addTag(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->addTag(getString(vm, 2));
            return 0;
        }

        SQRESULT removeTag(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->removeTag(getString(vm, 2));
            return 0;
        }

        SQRESULT getChildCount(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushinteger(vm, self->getChildCount());
            return 1;
        }

        SQRESULT getChild(HSQUIRRELVM vm)
        {
            GET_SELF();
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
                ss << "Invalid argument type to " << SN_FUNCNAME << " (got " << squirrel::getTypeName(t) << ")";
                std::string s = ss.str();
                return sq_throwerror(vm, s.c_str());
            }
        }

        SQRESULT setUpdatable(HSQUIRRELVM vm)
        {
            GET_SELF();
            bool enable = getBool(vm, 2);
            self->setUpdatable(enable);
            return 0;
        }

        SQRESULT getScript(HSQUIRRELVM vm)
        {
            GET_SELF();
            squirrel::Instance & obj = self->getScript();
            if (obj.isNull() || obj.getVM() != vm)
                sq_pushnull(vm);
            else
                sq_pushobject(vm, obj.getObject());
            return 1;
        }

		SQRESULT createChild(HSQUIRRELVM vm)
		{
			GET_SELF();
			SQInteger argType = sq_gettype(vm, 2);
			ScriptableObject * child = nullptr;
			if (argType == OT_STRING)
			{
				const SQChar * typeName = nullptr;
				if(SQ_SUCCEEDED(sq_getstring(vm, 2, &typeName)))
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

        SQRESULT instantiateChild(HSQUIRRELVM vm)
        {
            GET_SELF();
            const char * packName = squirrel::getString(vm, 2);
            AssetLocation location(packName);
            PackedEntity * pack = AssetDatabase::get().getAsset<PackedEntity>(location);
            if (pack)
            {
                std::vector<Entity*> entities;
                pack->instantiate(*self, location.project, &entities);
                if (!entities.empty())
                {
                    squirrel::push(vm, entities[0]);
                    return 1;
                }
            }
            else
            {
                std::stringstream ss;
                ss << "Asset not found: \"" << packName << '"';
                sq_throwerror(vm, ss.str().c_str());
            }
            return 0;
        }

        SQRESULT getIndexInParent(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushinteger(vm, self->getIndexInParent());
            return 1;
        }

        SQRESULT destroyChildren(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->destroyChildren();
            return 0;
        }

	}

void bindEntity(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Entity>(vm)
        .setMethod("getScene", getScene)
        .setMethod("getParent", getParent)
        .setMethod("getChild", getChild)
        .setMethod("getChildCount", getChildCount)
        .setMethod("getName", getName)
        .setMethod("setName", setName, 2, "xs")
        .setMethod("isEnabled", isEnabled)
        .setMethod("setEnabled", setEnabled, 2, "xb")
        .setMethod("hasTag", hasTag)
        .setMethod("addTag", addTag, 2, "xs")
        .setMethod("removeTag", removeTag)
        .setMethod("getScript", getScript)
        .setMethod("setUpdatable", setUpdatable, 2, "xb")
        .setMethod("createChild", createChild, -1, "xs")
        .setMethod("instantiateChild", instantiateChild, 2, "xs")
        .setMethod("getIndexInParent", getIndexInParent)
        .setMethod("destroyChildren", destroyChildren)
	;
}

} // namespace sn


