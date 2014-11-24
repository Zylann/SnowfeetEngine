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
bool ObjectTypeDatabase::isRegistered(ObjectType & t)
{
    return m_nameToID.find(t.name) != m_nameToID.end();
}

//------------------------------------------------------------------------------
Object * ObjectTypeDatabase::instantiate(const std::string className)
{
    auto it = m_factories.find(className);
#ifdef SN_BUILD_DEBUG
    if(it == m_factories.end())
    {
        SN_ERROR("cannot instantiate " << className << ", factory not found");
    }
#endif
    assert(it != m_factories.end());
    return it->second();
}

//------------------------------------------------------------------------------
ObjectType * ObjectTypeDatabase::getType(const std::string & typeName)
{
    auto it = m_nameToID.find(typeName);
    if(it != m_nameToID.end())
    {
        return m_registeredTypes[it->second];
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
    if (isRegistered(t))
    {
        ObjectTypeID id = m_nameToID.find(t.name)->second;

        m_nameToID.erase(t.name);
        m_factories.erase(t.name);
        m_registeredTypes[id] = nullptr;

        while ((!m_registeredTypes.empty()) && m_registeredTypes.back() == nullptr)
            m_registeredTypes.pop_back();
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
        ObjectType * t = *it;
        if (t && t->moduleName == name)
        {
            unregisterType(*t);
            ++count;
        }
    }

    if (count == 0)
    {
        SN_WARNING("ObjectTypeDatabase::unregisterModule: no type found in module \"" << name << "\"");
    }
}

} // namespace sn

