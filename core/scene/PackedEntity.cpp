#include "PackedEntity.h"
#include <core/asset/AssetDatabase.h>
#include "helpers/DumpToObjectDBConverter.h"
#include "../util/stringutils.h"
#include "Scene.h"

// Legacy
#include "SquirrelComponent.h"

namespace sn
{

//------------------------------------------------------------------------------
SN_OBJECT_IMPL(PackedEntity)

//------------------------------------------------------------------------------
const char * PackedEntity::CHILDREN_TAG = "_children";
const char * PackedEntity::COMPONENTS_TAG = "_components";
const char * PackedEntity::TYPE_TAG = "@type";

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
void PackedEntity::instantiate(
        Entity & a_parent, 
        const std::string & contextProjectName, 
        std::vector<Entity*> * out_rootEntities
        )
{
    if (!isFlattened())
        flatten();

    instantiateOnly(a_parent, contextProjectName, out_rootEntities);
}

//------------------------------------------------------------------------------
void PackedEntity::instantiateOnly(
        Entity & a_parent, 
        const std::string & contextProjectName, 
        std::vector<Entity*> * out_rootEntities
        ) const
{
    // Get scene
    Scene * scenePtr = sn::Object::cast<Scene>(&a_parent);
    if (scenePtr == nullptr)
        scenePtr = a_parent.getScene();
    SN_ASSERT(scenePtr != nullptr, "Cannot access the scene");
    Scene & scene = *scenePtr;

    SerializationContext context(contextProjectName);
    ObjectTypeDatabase & typeDatabase = ObjectTypeDatabase::get();

    const auto & objectDatas = getObjects();

    // TODO Entity order inside children vector is not guaranteed!

    const ObjectType & entityT = sn::getObjectType<Entity>();
    const ObjectType & componentT = sn::getObjectType<Component>();

    // We'll fill the map of objects within the context so state deserialization will have access to them
    std::unordered_map<u32, sn::Object*> & objects = context.getObjectMap();

    // Entity instance ; entity data
    std::vector< std::pair<Entity*, const Variant*> > entities;
    // Component instance ; component data
    std::vector< std::pair<Component*, const Variant*> > components;

    // Create entity instances first
    for (auto it = objectDatas.begin(); it != objectDatas.end(); ++it)
    {
        u32 id = it->first;
        const Variant & o = it->second.data;

        std::string typeName = o["@type"].getString();
        const ObjectType * ot = typeDatabase.getType(typeName);
        if (ot)
        {
            if (ot->isAbstract())
                continue;

            if (ot->derivesFrom(entityT))
            {
                // Instantiate entity within the scene so it gets an ID and a reference to the scene
                sn::Entity * entity = scene.createChild(*ot);
                if (entity)
                {
                    objects[id] = entity;
                    entities.push_back(std::make_pair(entity, &o));

                    instantiateComponents(*entity, o, objects, components);
                }
            }
            else
            {
                SN_ERROR("Wrong object type in " << sn::toString(getAssetMetadata().path) << ": " << ot->toString());
            }
        }
        else
        {
            SN_ERROR("Unknown object type from JSON (name=" << typeName << ")");
        }
    }

    // Rebuild children hierarchy
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        // For each entity
        Entity & entity = *it->first;
        const Variant & o = *it->second;
        rebuildHierarchy(entity, o, objects);
    }
    // Place parent-less entities under the root
    // TODO Optimize with a _parent field inside the data, in addition to _children?
    // TODO Remove that code duplication?
    std::vector<Entity*> rootEntities_;
    std::vector<Entity*> & rootEntities = out_rootEntities ? *out_rootEntities : rootEntities_;
    if (&a_parent == &scene)
    {
        // Just collect root entities
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            Entity * e = it->first;
            if (e->getParent() == &scene)
            {
                rootEntities.push_back(e);
            }
        }
    }
    else
    {
        // Collect root entities and set their parent
        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            Entity * e = it->first;
            if (e->getParent() == &scene)
            {
                e->setParent(&a_parent);
                rootEntities.push_back(e);
            }
        }
    }

    // Deserialize states
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        Entity * entity = it->first;
        const Variant * o = it->second;
        entity->unserializeState(*o, context);

        // Legacy support, will be removed soon!
        const Variant & scriptData = (*o)["script"];
        if (scriptData.isDictionary())
        {
            SquirrelComponent * cmp = entity->addComponent<SquirrelComponent>();
            cmp->unserializeState(scriptData, context);
            SN_WARNING("PackedEntity legacy support: the \"script\" field on entities is deprecated, use components instead");
        }
    }
    for (auto it = components.begin(); it != components.end(); ++it)
    {
        Component * e = it->first;
        const Variant * o = it->second;
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
void PackedEntity::rebuildHierarchy(
    Entity & parent,
    const Variant & entityData,
    std::unordered_map<u32,sn::Object*> objects) const
{
    // Get children array
    const Variant & childrenValue = entityData[CHILDREN_TAG];
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
                auto childIt = objects.find(parentID);
                if (childIt != objects.end())
                {
                    Entity * child = sn::Object::cast<Entity>(childIt->second);
                    if (child)
                    {
                        // Set child parent
                        child->setParent(&parent);
                    }
                    else
                    {
                        SN_ERROR("Wrong child type: " << child->getObjectType().toString());
                    }
                }
            }
            //else
            //{
            //    // The element is not a reference...
            //}
        }
    }
}

//------------------------------------------------------------------------------
void PackedEntity::instantiateComponents(
    Entity & entity,
    const Variant & entityData,
    std::unordered_map<u32, sn::Object*> & out_objects,
    std::vector< std::pair<Component*, const Variant*> > & out_components) const
{
    const Variant & componentsValue = entityData[COMPONENTS_TAG];
    if (componentsValue.isArray())
    {
        ObjectTypeDatabase & typeDatabase = ObjectTypeDatabase::get();

        // For each component
        const Variant::Array & componentsArray = componentsValue.getArray();
        for (size_t i = 0; i < componentsArray.size(); ++i)
        {
            const Variant & componentRef = componentsArray[i];
            u32 ref = 0;
            if (ObjectDB::getRef(componentRef, ref))
            {
                const Variant * componentValue = getObject(ref);
                if (componentValue)
                {
                    std::string cmpTypeName = (*componentValue)["@type"].getString();
                    const ObjectType * cmpType = typeDatabase.getType(cmpTypeName);
                    if (cmpType)
                    {
                        if (cmpType->isAbstract())
                            continue;

                        Component * cmp = entity.addComponent(*cmpType);
                        if (cmp)
                        {
                            out_objects[ref] = cmp;
                            out_components.push_back(std::make_pair(cmp, componentValue));
                        }
                        else
                        {
                            SN_ERROR("Couldn't add " << cmpType->toString() << " as a component type");
                        }
                    }
                }
            }
            //else
            //{
            //    // The element is not a reference...
            //}
        }
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

