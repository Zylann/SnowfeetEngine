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

} // namespace sn

