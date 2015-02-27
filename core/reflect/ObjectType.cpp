/*
ObjectType.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ObjectType.hpp"
#include "ObjectTypeDatabase.hpp"

#include <sstream>

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
    if(m_ID == 0)
    {
        // Always return false
        return false;
    }

    if(m_ID == other.m_ID)
    {
        // Types exactly match
        return true;
    }
    else if(includeInheritance)
    {
        // Search inheritance tree

        ObjectTypeDatabase & odb = ObjectTypeDatabase::get();
        const ObjectType * baseType = this;

        while(!baseType->m_baseName.empty())
        {
            baseType = odb.getType(baseType->m_baseName);
            if(baseType == nullptr)
                return false;
            if(baseType->m_ID == other.m_ID)
                return true;
        }
    }

    // Types differ
    return false;
}

//------------------------------------------------------------------------------
bool ObjectType::hasProperty(const std::string & name) const
{
    return m_properties.find(name) != m_properties.end();
}

//------------------------------------------------------------------------------
const ObjectProperty * ObjectType::getProperty(const std::string & name) const
{
    auto it = m_properties.find(name);
    if (it != m_properties.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
Object * ObjectType::instantiate() const
{
    Object * obj = m_factory();
#ifdef SN_BUILD_DEBUG
    if (obj == nullptr)
    {
        SN_ERROR("ObjectType::instantiate: couldn't create instance of " << toString());
    }
#endif
    return obj;
}

//------------------------------------------------------------------------------
std::string ObjectType::toString() const
{
    std::stringstream ss;
    ss << "{[" << m_ID << "]" << m_name << " : " << m_baseName << "}";
    return ss.str();
}

//------------------------------------------------------------------------------
bool operator==(const ObjectType & a, const ObjectType & b)
{
	// TODO Compare modules too?
    return a.getName() == b.getName();
}

//------------------------------------------------------------------------------
bool operator!=(const ObjectType & a, const ObjectType & b)
{
    return a.getName() != b.getName();
}

} // namespace sn

