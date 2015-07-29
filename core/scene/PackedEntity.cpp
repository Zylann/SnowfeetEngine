#include "PackedEntity.h"
#include <core/asset/AssetDatabase.h>
#include "helpers/DumpToObjectDBConverter.h"

namespace sn
{

//------------------------------------------------------------------------------
const char * PackedEntity::CHILDREN_TAG = "_children";

//------------------------------------------------------------------------------
bool PackedEntity::loadFromJSON(JsonBox::Value & doc)
{
    std::string format = doc[ObjectDB::FORMAT_KEY].getString();
    if (format == ObjectDB::FORMAT_VALUE)
        return ObjectDB::loadFromJSON(doc);
    else
        return loadFromLegacyDump(doc);
}

//------------------------------------------------------------------------------
bool PackedEntity::loadFromLegacyDump(JsonBox::Value & input)
{
    DumpToObjectDBConverter converter;
    converter.convert(input);
    //converter.output.writeToFile("dump.json", true);
    return ObjectDB::loadFromJSON(converter.output);
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
void PackedEntity::instantiate(Entity & a_parent, const std::string & contextModuleName)
{
    SerializationContext context(contextModuleName);

    // TODO Flatten before?

    auto & objects = getObjects();

    // TODO Entity order inside children vector is not guaranteed!

    const ObjectType & entityT = sn::getObjectType<Entity>();
    std::unordered_map<u32, sn::Object*> & entities = context.getObjectMap();

    // Create instances first
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        u32 id = it->first;
        JsonBox::Value & o = it->second.data;

        std::string typeName = o[SN_JSON_TYPE_TAG].getString();
        ObjectType * ot = ObjectTypeDatabase::get().getType(typeName);
        if (ot)
        {
            sn::Object * obj = instantiateDerivedObject(*ot, entityT);
            if (obj)
            {
                entities[id] = (Entity*)obj;
            }
        }
        else
        {
            SN_ERROR("Unknown object object type from JSON (name=" << typeName << ")");
        }
    }

    // Rebuild children hierarchy
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        // For each entity
        Entity * e = (Entity*)it->second;
        JsonBox::Value * o = getObject(it->first);
        SN_ASSERT(o != nullptr, "Invalid state");

        // Get children array
        JsonBox::Value & childrenValue = (*o)[CHILDREN_TAG];
        if (childrenValue.isArray())
        {
            // For each child element
            size_t count = childrenValue.getArray().size();
            for (size_t i = 0; i < count; ++i)
            {
                JsonBox::Value & childValue = childrenValue[i];
                u32 parentID = 0;
                if (ObjectDB::getRef(childValue, parentID))
                {
                    auto childIt = entities.find(parentID);
                    if (childIt != entities.end())
                    {
                        // Set child parent
                        Entity * child = (Entity*)childIt->second;
                        child->setParent(e);
                    }
                }
                //else
                //{
                //    // The element is not a reference...
                //}
            }
        }

        //JsonBox::Value & parentValue = (*o)["parent"];
        //
        //u32 parentIndex = -1;
        //JsonBox::Value & parentIndexValue = (*o)["parentIndex"];
        //if (parentIndexValue.isInteger())
        //{
        //    parentIndexValue = parentIndexValue.getInt();
        //}

        //if (parentValue.isNull())
        //{
        //    e->setParent(&a_parent, parentIndex);
        //    rootEntities.push_back(e);
        //}
        //else
        //{
        //    u32 parentID = 0;
        //    if (ObjectDB::getRef(parentValue, parentID))
        //    {
        //        auto it = entities.find(parentID);
        //        if (it != entities.end())
        //        {
        //            Entity * parent = (Entity*)it->second;
        //            e->setParent(parent, parentIndex);
        //        }
        //    }
        //}
    }
    std::vector<Entity*> rootEntities;
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        Entity * e = (Entity*)it->second;
        if (e->getParent() == nullptr)
        {
            e->setParent(&a_parent);
            rootEntities.push_back(e);
        }
    }

    // Deserialize states
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        Entity * e = (Entity*)it->second;
        JsonBox::Value * o = getObject(it->first);
        e->unserializeState(*o, context);
    }

    // Trigger onReady() event
    for (auto it = rootEntities.begin(); it != rootEntities.end(); ++it)
    {
        Entity * e = *it;
        e->propagateOnReady();
    }
}

//------------------------------------------------------------------------------
ObjectDB * PackedEntity::getFromAssetDatabase(const std::string & location) const
{
    return AssetDatabase::get().getAsset<PackedEntity>(AssetLocation(location));
}

} // namespace sn

