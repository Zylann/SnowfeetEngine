#include <core/util/assert.hpp>

#include "Entity.hpp"
#include "TagManager.hpp"

namespace sn
{

//------------------------------------------------------------------------------
TagManager & TagManager::setTag(u8 index, const std::string & name)
{
    // TODO there is something weird with these log messages
#ifdef SN_BUILD_DEBUG
    if(index >= MAX_TAGS)
    {
        SN_ERROR("TagManager::setTag: given index (" << (u32)index << ", name: \"" << name << "\")");
    }
#endif
    SN_ASSERT(index < MAX_TAGS, "setTag: tag index is too high! (" << (u32)index << ", name: \"" << name << "\")");

    auto it = m_name2Tag.find(name);
#ifdef SN_BUILD_DEBUG
    if(it != m_name2Tag.end())
    {
        SN_ERROR("TagManager::setTag: two tags cannot have the same name! \"" << name << "\"");
    }
#endif
    SN_ASSERT(it == m_name2Tag.end(), "TagManager::setTag: two tags cannot have the same name! \"" << name << "\"");

    m_tags[index] = name;

    return *this;
}

//------------------------------------------------------------------------------
const std::string & TagManager::nameFromIndex(u8 index) const
{
#ifdef SN_BUILD_DEBUG
    if(index >= MAX_TAGS)
    {
        SN_ERROR("TagManager::nameFromIndex: given index (" << (u32)index << ") is out of bounds!");
    }
#endif
    SN_ASSERT(index < MAX_TAGS, "TagManager::nameFromIndex: given index (" << (u32)index << ") is out of bounds!");
    return m_tags[index];
}

//------------------------------------------------------------------------------
s8 TagManager::indexFromName(const std::string& name) const
{
    auto it = m_name2Tag.find(name);
    if(it == m_name2Tag.end())
    {
        SN_ERROR("TagManager::indexFromName: undefined tag name \"" << name << "\"");
        return -1;
    }
    return it->second;
}

//------------------------------------------------------------------------------
const std::unordered_set<Entity*> & TagManager::taggedEntities(u8 tagIndex) const
{
#ifdef SN_BUILD_DEBUG
    if(tagIndex >= MAX_TAGS)
    {
        SN_ERROR("TagManager::getTaggedEntities: given index (" << (u32)tagIndex << ") is out of bounds!");
    }
#endif
    SN_ASSERT(tagIndex < MAX_TAGS, "TagManager::getTaggedEntities: given index (" << (u32)tagIndex << ") is out of bounds!");
    return m_taggedEntities[tagIndex];
}

//------------------------------------------------------------------------------
void TagManager::onEntityTagged(Entity* e, u8 tagIndex)
{
    m_taggedEntities[tagIndex].insert(e);
}

//------------------------------------------------------------------------------
void TagManager::onEntityUntagged(Entity* e, u8 tagIndex)
{
    m_taggedEntities[tagIndex].erase(e);
}

//------------------------------------------------------------------------------
void TagManager::print(std::ostream& os) const
{
    os << "---------- TagManager ----------" << std::endl;
    for(u32 i = 0; i < MAX_TAGS; ++i)
    {
        os << "[" << i << "] \"" << m_tags[i] << "\", "
            << m_taggedEntities[i].size() << " entities" << std::endl;
    }
    os << "----------" << std::endl;
}

//------------------------------------------------------------------------------
void TagManager::serialize(JsonBox::Value & o)
{
    JsonBox::Array a;
    a.resize(MAX_TAGS);
    for(u32 i = 0; i < MAX_TAGS; ++i)
    {
        a[i] = m_tags[i];
    }
    o = a;
}

//------------------------------------------------------------------------------
void TagManager::unserialize(JsonBox::Value & o)
{
    const u32 n = o.getArray().size();
    for(u32 i = 0; i < n; ++i)
    {
        setTag(i, o[i].getString());
    }
}

} // namespace sn



