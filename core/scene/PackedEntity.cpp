#include "PackedEntity.h"
#include <core/asset/AssetDatabase.h>
#include "helpers/DumpToObjectDBConverter.h"

namespace sn
{

//------------------------------------------------------------------------------
const char * PackedEntity::CHILDREN_TAG = "_children";

//------------------------------------------------------------------------------
bool PackedEntity::loadFromVariant(Variant & doc)
{
    if (!doc.isDictionary())
        return false;
    const Variant & formatElem = doc[ObjectDB::FORMAT_KEY];
    if (formatElem.isString() && formatElem.getString() == ObjectDB::FORMAT_VALUE)
        return ObjectDB::loadFromVariant(doc);
    else
        return loadFromLegacyDump(doc);
}

//------------------------------------------------------------------------------
bool PackedEntity::loadFromLegacyDump(Variant & input)
{
    DumpToObjectDBConverter converter;
    converter.convert(input);
    //converter.output.writeToFile("dump.json", true);
    return ObjectDB::loadFromVariant(converter.output);
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
const ObjectType * PackedEntity::getRootType() const
{
    const Variant * root = getRootObject();
    if (root)
    {
        const Variant & typeTag = (*root)["@type"];
        if (typeTag.isString())
        {
            const ObjectType * ot = ObjectTypeDatabase::get().getType(typeTag.getString());
            return ot;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void PackedEntity::instantiate(Entity & a_parent, const std::string & contextModuleName)
{
    if (!isFlattened())
        flatten();

    instantiateOnly(a_parent, contextModuleName);
}

//------------------------------------------------------------------------------
void PackedEntity::instantiateOnly(Entity & a_parent, const std::string & contextModuleName) const
{
    SerializationContext context(contextModuleName);

    const auto & objects = getObjects();

    // TODO Entity order inside children vector is not guaranteed!

    const ObjectType & entityT = sn::getObjectType<Entity>();
    std::unordered_map<u32, sn::Object*> & entities = context.getObjectMap();

    // Create instances first
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        u32 id = it->first;
        const Variant & o = it->second.data;

        std::string typeName = o["@type"].getString();
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
        const Variant * o = getObject(it->first);
        SN_ASSERT(o != nullptr, "Invalid state");

        // Get children array
        const Variant & childrenValue = (*o)[CHILDREN_TAG];
        if (childrenValue.isArray())
        {
            // For each child element
            const Variant::Array & childrenArray = childrenValue.getArray();
            for (size_t i = 0; i < childrenArray.size(); ++i)
            {
                const Variant & childValue = childrenArray[i];
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
        const Variant * o = getObject(it->first);
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
    AssetLocation loc(location);
    if (loc.project.empty())
    {
        loc.project = getAssetMetadata().project;
    }
    return AssetDatabase::get().getAsset<PackedEntity>(loc);
}

} // namespace sn

