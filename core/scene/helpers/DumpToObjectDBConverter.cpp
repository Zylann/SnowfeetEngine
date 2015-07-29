#include "DumpToObjectDBConverter.h"
#include <core/asset/ObjectDB.h>
#include "../PackedEntity.h"

#ifdef SN_BUILD_DEBUG
#include <core/system/time/Clock.h>
#endif

namespace sn
{

//------------------------------------------------------------------------------
void DumpToObjectDBConverter::convert(JsonBox::Value & input)
{
#ifdef SN_BUILD_DEBUG
    Clock profileClock;
#endif

    // Initialize
    m_nextID = 0;
    output.setObject(JsonBox::Object());

    JsonBox::Array objects;

    JsonBox::Value & inputEntitiesValue = input["entities"];
    if (!inputEntitiesValue.isArray())
        return;

    // Root objects
    size_t count = inputEntitiesValue.getArray().size();
    for (size_t i = 0; i < count; ++i)
    {
        JsonBox::Value & srcObj = inputEntitiesValue[i];
        if (!srcObj.isObject())
            continue;

        JsonBox::Value dstObj = srcObj;
        extractChildren(dstObj, objects);

        objects.push_back(static_cast<s32>(makeID()));
        objects.push_back(dstObj);
    }
    
    output[ObjectDB::OBJECTS_TAG].setArray(objects);
    output[ObjectDB::FORMAT_KEY].setString(ObjectDB::FORMAT_VALUE);
    output[ObjectDB::ROOT_ID_TAG].setInt(0);

#ifdef SN_BUILD_DEBUG
    SN_DLOG("Dump -> ObjectDB conversion time: " << profileClock.getElapsedTime().asSeconds() << "s");
#endif
}

//------------------------------------------------------------------------------
u32 DumpToObjectDBConverter::makeID()
{
    return m_nextID++;
}

//------------------------------------------------------------------------------
void DumpToObjectDBConverter::extractChildren(JsonBox::Value & obj, JsonBox::Array & objectsArray)
{
    SN_ASSERT(obj.isObject(), "Expected JSON object");
    auto childIt = obj.getObject().find(PackedEntity::CHILDREN_TAG);
    if (childIt == obj.getObject().end())
        return;
    JsonBox::Value & childrenValue = obj[PackedEntity::CHILDREN_TAG];
    if (!childrenValue.isArray())
        return;

    size_t count = childrenValue.getArray().size();
    for (size_t i = 0; i < count; ++i)
    {
        JsonBox::Value & child = childrenValue[i];
        if (!child.isObject())
            continue;

        // Extract children first so we won't copy unnecessary child data
        extractChildren(child, objectsArray);

        // Place the child in the objects array
        u32 childID = makeID();
        objectsArray.push_back(static_cast<s32>(childID));
        objectsArray.push_back(child.getObject()); // copy

        // Replace the child by a reference
        JsonBox::Object childRef;
        childRef[ObjectDB::REF_TAG].setInt(childID);
        childrenValue[i] = childRef;
    }
}

} // namespace sn

