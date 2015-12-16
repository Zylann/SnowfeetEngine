#include "DumpToObjectDBConverter.h"
#include <core/asset/ObjectDB.h>
#include "../PackedEntity.h"

#ifdef SN_BUILD_DEBUG
#include <core/system/Clock.h>
#endif

namespace sn
{

//------------------------------------------------------------------------------
void DumpToObjectDBConverter::convert(const Variant & input)
{
#ifdef SN_BUILD_DEBUG
    Clock profileClock;
#endif

    // Initialize
    m_nextID = 0;
    output.setDictionary();

    if (!input.isDictionary())
        return;

    Variant::Array objects;

    const Variant & inputEntitiesValue = input["entities"];
    if (!inputEntitiesValue.isArray())
        return;

    // Root objects
    const Variant::Array & inputEntitiesArray = inputEntitiesValue.getArray();
    for (size_t i = 0; i < inputEntitiesArray.size(); ++i)
    {
        const Variant & srcObj = inputEntitiesArray[i];
        if (!srcObj.isDictionary())
            continue;

        Variant dstObj = srcObj;
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
void DumpToObjectDBConverter::extractChildren(Variant & inout_obj, Variant::Array & out_objectsArray)
{
    SN_ASSERT(inout_obj.isDictionary(), "Expected Variant object type");
    auto childIt = inout_obj.getDictionary().find(PackedEntity::CHILDREN_TAG);
    if (childIt == inout_obj.getDictionary().end())
        return;
    Variant & childrenValue = inout_obj[PackedEntity::CHILDREN_TAG];
    if (!childrenValue.isArray())
        return;

    Variant::Array & childrenArray = childrenValue.getArray();
    for (size_t i = 0; i < childrenArray.size(); ++i)
    {
        Variant & child = childrenArray[i];
        if (!child.isDictionary())
            continue;

        // Extract children first so we won't copy unnecessary child data
        extractChildren(child, out_objectsArray);

        // Place the child in the objects array
        u32 childID = makeID();
        out_objectsArray.push_back(static_cast<s32>(childID));
        out_objectsArray.push_back(child.getDictionary()); // copy

        // Replace the child by a reference
        // TODO Refs should be represented by integers with one bit set in Variant type extra data
        Variant childRef;
        childRef.setDictionary();
        childRef[ObjectDB::REF_TAG].setInt(childID);
        childrenArray[i] = childRef;
    }
}

} // namespace sn

