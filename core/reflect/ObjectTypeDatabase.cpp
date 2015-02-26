/*
ObjectTypeDatabase.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <cassert>
#include "ObjectTypeDatabase.hpp"

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
bool ObjectTypeDatabase::isRegistered(const std::string & typeName)
{
    return m_registeredTypes.find(typeName) != m_registeredTypes.end();
}

//------------------------------------------------------------------------------
ObjectType * ObjectTypeDatabase::getType(const std::string & typeName)
{
    auto it = m_registeredTypes.find(typeName);
    if (it != m_registeredTypes.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
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
void ObjectTypeDatabase::unregisterType(ObjectType & t)
{
    auto it = m_registeredTypes.find(t.getName());
    if (it != m_registeredTypes.end())
    {
        delete it->second;
        m_registeredTypes.erase(it);
    }
    else
    {
        SN_ERROR("ObjectTypeDatabase::unregisterType: type not found (" << t.toString() << ")");
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

    auto types = m_registeredTypes;
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        ObjectType * t = it->second;
        if (t->getModuleName() == name)
        {
            unregisterType(*t);
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
    for (auto it = m_registeredTypes.begin(); it != m_registeredTypes.end(); ++it)
    {
        delete it->second;
    }
    m_registeredTypes.clear();
    m_nextID = 1;
}

} // namespace sn

