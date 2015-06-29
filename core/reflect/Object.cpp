/*
Object.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Object.h"

#ifdef SN_BUILD_DEBUG
#include <unordered_set>
#include <core/system/thread/Mutex.h>
#endif

namespace sn
{

#ifdef SN_BUILD_DEBUG
    
    namespace
    {
        std::unordered_set<Object*> s_allObjects;
        Mutex s_allObjectsMutex;
    }

    Object::Object()
    {
        Lock l(s_allObjectsMutex);
        auto & allObjects = s_allObjects;
        allObjects.insert(this);
    }

    Object::~Object()
    {
        Lock l(s_allObjectsMutex);
        auto & allObjects = s_allObjects;
        allObjects.erase(this);
    }

    // Static
    u32 Object::getInstanceCount()
    {
        Lock l(s_allObjectsMutex);
        return s_allObjects.size();
    }

    // Static
    void Object::printInstances()
    {
        Lock l(s_allObjectsMutex);
        std::stringstream ss;
        for (auto it = s_allObjects.begin(); it != s_allObjects.end(); ++it)
        {
            Object * obj = *it;
            ss << obj->getObjectType().getName() << std::endl;
        }
        SN_LOG("Objects:\n" << ss.str());
    }

#endif

Object * instantiateDerivedObject(const std::string & typeName, const std::string & derivedTypeName)
{
    ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    ObjectType * type = otb.getType(typeName);
    if (type)
    {
        ObjectType * derived = otb.getType(derivedTypeName);
        if (derived)
        {
            if (type->is(*derived))
            {
                Object * obj = type->instantiate();
                if (obj)
                    return obj;
                else
                    SN_ERROR("Cannot instantiate object type '" << type->toString() << "'");
            }
            else
                SN_ERROR("Type '" << type->toString() << "' doesn't derives from '" << derived->toString() << "'");
        }
        else
            SN_ERROR("Cannot instantiate object type '" << type->toString() << "', derived type '" << derivedTypeName << "' is not registered");
    }
    else
        SN_ERROR("Cannot instantiate unregistered type '" << typeName << "'");
    return nullptr;
}

} // namespace sn

