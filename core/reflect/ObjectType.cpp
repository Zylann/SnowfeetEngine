/*
ObjectType.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ObjectType.hpp"
#include "ObjectTypeDatabase.hpp"

namespace sn
{

//------------------------------------------------------------------------------
bool ObjectType::is(const std::string & typeName, bool includeInheritance) const
{
    ObjectType * t = ObjectTypeDatabase::get().getType(typeName);
    if (t)
        return is(*t, includeInheritance);
    else
        return false;
}

//------------------------------------------------------------------------------
bool ObjectType::is(const ObjectType & other, bool includeInheritance) const
{
    // If the object has a Null type
    if(ID == 0)
    {
        // Always return false
        return false;
    }

    if(ID == other.ID)
    {
        // Types exactly match
        return true;
    }
    else if(includeInheritance)
    {
        // Search inheritance tree

        ObjectTypeDatabase & odb = ObjectTypeDatabase::get();
        const ObjectType * baseType = this;

        while(!baseType->baseName.empty())
        {
            baseType = odb.getType(baseType->baseName);
            if(baseType == nullptr)
                return false;
            if(baseType->ID == other.ID)
                return true;
        }
    }

    // Types differ
    return false;
}

//------------------------------------------------------------------------------
Object * ObjectType::instantiate()
{
    Object * obj = factory();
#ifdef SN_BUILD_DEBUG
    if (obj == nullptr)
    {
        SN_ERROR("ObjectType::instantiate: couldn't create instance of " << toString());
    }
#endif
    return obj;
}

} // namespace sn

