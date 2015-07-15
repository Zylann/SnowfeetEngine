#include "PackedEntity.h"

#define SN_JSON_ENTITY_CHILDREN_TAG "_children"

namespace sn
{

//------------------------------------------------------------------------------
bool PackedEntity::loadFromStream(std::istream & is)
{
    m_jsonData.loadFromStream(is);
    return true;
}

//------------------------------------------------------------------------------
bool PackedEntity::saveToStream(std::ostream & os)
{
    m_jsonData.writeToStream(os, false);
    return true;
}

//------------------------------------------------------------------------------
void PackedEntity::loadFromInstance(const Entity & entity)
{
    // TODO

    // Old code from Scene
    //JsonBox::Value doc;
    //JsonBox::Value & docEntities = doc["entities"];
    //for (u32 i = 0; i < getChildCount(); ++i)
    //{
    //    Entity::serialize(docEntities[i], *getChildByIndex(i), context);
    //}
}

//------------------------------------------------------------------------------
void PackedEntity::instantiate(Entity & a_parent, const SerializationContext & context)
{
    JsonBox::Value & docEntities = m_jsonData["entities"];

    if (docEntities.isArray())
    {
        u32 len = docEntities.getArray().size();
        for (u32 i = 0; i < len; ++i)
        {
            // Note: the returned child will be automatically added to the children list,
            // as soon as setParent() is called
            Entity * e = PackedEntity::unserialize(docEntities[i], &a_parent, context);
		    e->propagateOnReady();
        }
    }
}

//------------------------------------------------------------------------------
// Static
Entity * PackedEntity::unserialize(JsonBox::Value & o, Entity * parent, const SerializationContext & context)
{
    std::string typeName = o[SN_JSON_TYPE_TAG].getString();
    ObjectType * ot = ObjectTypeDatabase::get().getType(typeName);
    if (ot)
    {
        Object * obj = instantiateDerivedObject(typeName, Entity::__sGetClassName());
        if (obj)
        {
            Entity * e((Entity*)obj);
            e->setParent(parent);
            e->unserializeState(o, context);

            if (o[SN_JSON_ENTITY_CHILDREN_TAG].isArray())
            {
                JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
                u32 len = a.getArray().size();
                for (u32 i = 0; i < len; ++i)
                {
                    PackedEntity::unserialize(a[i], e, context);
                }
            }

            return e;
        }
        // Error message already handled by the instantiate helper
    }
    else
    {
        SN_ERROR("Unknown object object type from JSON (name=" << typeName << ")");
    }
    return nullptr;
}

//------------------------------------------------------------------------------
// Static
void PackedEntity::serialize(JsonBox::Value & o, Entity & e, const SerializationContext & context)
{
    o[SN_JSON_TYPE_TAG] = Entity::__sGetClassName();
    e.serializeState(o, context);
    if (e.getChildCount() != 0)
    {
        JsonBox::Value & a = o[SN_JSON_ENTITY_CHILDREN_TAG];
        for (u32 i = 0; i < e.getChildCount(); ++i)
        {
            Entity * child = e.getChildByIndex(i);
            PackedEntity::serialize(a[i], *child, context);
        }
    }
}

} // namespace sn

