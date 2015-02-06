/*
Object.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Object.hpp"

namespace sn
{

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

