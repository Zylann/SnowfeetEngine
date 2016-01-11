/*
ObjectTypeDatabase.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <cassert>
#include "ObjectTypeDatabase.h"

namespace sn
{

//------------------------------------------------------------------------------
// Static
ObjectTypeDatabase & ObjectTypeDatabase::get()
{
    static ObjectTypeDatabase s_instance;
    return s_instance;
}

//------------------------------------------------------------------------------
// Private to prevent construction from the outside
ObjectTypeDatabase::ObjectTypeDatabase()
{}

//------------------------------------------------------------------------------
ObjectTypeDatabase::~ObjectTypeDatabase()
{
    clear();
}

//------------------------------------------------------------------------------
bool ObjectTypeDatabase::isRegistered(const std::string & typeName)
{
    return m_typesByName.find(typeName) != m_typesByName.end();
}

//------------------------------------------------------------------------------
const ObjectType * ObjectTypeDatabase::getType(const std::string & typeName)
{
    auto it = m_typesByName.find(typeName);
    if (it != m_typesByName.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
const ObjectType * ObjectTypeDatabase::getType(u32 id)
{
    if (id < m_types.size())
        return m_types[id];
    return nullptr;
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::beginModule(const std::string & name)
{
    m_currentModule = name;
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::endModule()
{
    m_currentModule = "";
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::unregisterType(const ObjectType & t)
{
    if (t.getID() < m_types.size())
    {
        m_types[t.getID()] = nullptr;
    }
    else
    {
        SN_ERROR("ObjectTypeDatabase::unregisterType: type not found (" << t.toString() << ")");
    }
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::getTypesByModuleName(const std::string & modName, std::vector<const ObjectType*> & out_types) const
{
    for (auto it = m_types.begin(); it != m_types.end(); ++it)
    {
        const ObjectType * ot = *it;
        if (ot && ot->getModuleName() == modName)
        {
            out_types.push_back(ot);
        }
    }
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::unregisterModule(const std::string & name)
{
    if (name.empty())
    {
        SN_ERROR("ObjectTypeDatabase::unregisterModule: name is empty");
        return;
    }

    unsigned int count = 0;

    for (auto it = m_types.begin(); it != m_types.end(); ++it)
    {
        const ObjectType * t = *it;
        if (t && t->getModuleName() == name)
        {
            *it = nullptr;
            ++count;
        }
    }

    if (count == 0)
    {
        SN_LOG("ObjectTypeDatabase::unregisterModule: no type found in module \"" << name << "\"");
    }
}

//------------------------------------------------------------------------------
void ObjectTypeDatabase::clear()
{
    m_types.clear();
    m_typesByName.clear();
}

} // namespace sn

