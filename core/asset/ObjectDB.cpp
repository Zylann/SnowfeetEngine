#include "ObjectDB.h"
#include "../util/Log.h"
#include "../asset/AssetDatabase.h"
#include <core/sml/SmlParser.h>

#define VALIDATION_ERROR(msg)\
    SN_ERROR("Invalid " << ObjectDB::FORMAT_VALUE << " format: " << msg)

#define VALIDATION_WARNING(msg)\
    SN_WARNING("Invalid " << ObjectDB::FORMAT_VALUE << " format: " << msg)

namespace sn
{

//------------------------------------------------------------------------------
const char * ObjectDB::FORMAT_KEY     = "format";
const char * ObjectDB::FORMAT_VALUE   = "SN2";
const char * ObjectDB::SRC_TAG        = "@src";
const char * ObjectDB::REF_TAG        = "@ref";
const char * ObjectDB::CHANGES_TAG    = "changes";
const char * ObjectDB::OBJECTS_TAG    = "objects";
const char * ObjectDB::NEXT_ID_TAG    = "next";
const char * ObjectDB::ROOT_ID_TAG    = "root";

//------------------------------------------------------------------------------
ObjectDB::ObjectDB():
    m_nextID(0),
    m_rootID(NO_ROOT),
    m_isFlattened(false)
{
}

//------------------------------------------------------------------------------
bool ObjectDB::loadFromStream(std::istream & is)
{
    Variant doc;
    SmlParser parser;
    parser.parseValue(is, doc);
    return loadFromVariant(doc);
}

//------------------------------------------------------------------------------
bool ObjectDB::loadFromVariant(Variant & doc)
{
	// Validate
    u32 nextID = 0;
	if (!validate(doc, &nextID))
	{
		return false;
	}

    clear();

    m_nextID = nextID;

    // Get root ID
    Variant rootIDElem = doc[ROOT_ID_TAG];
    if (rootIDElem.isInt())
    {
        m_rootID = rootIDElem.getInt();
    }

    // TODO Allow single-object storage to ease configuration files

    // Store data
    const Variant & objects = doc[OBJECTS_TAG];
    const Variant::Array & objectsArray = objects.getArray();
    for (size_t i = 0; i < objectsArray.size(); i += 2)
    {
        // Get ID
        const Variant & idElem = objectsArray[i];
        u32 id = static_cast<u32>(idElem.getInt());

        // Store object
        const Variant::Dictionary & objElem = objectsArray[i + 1].getDictionary();
        auto srcIt = objElem.find(SRC_TAG);
        if (srcIt == objElem.end())
        {
            Object & obj = m_objects[id];
            obj.data = objElem;
        }
        else
        {
            OverrideObject & obj = m_overridedObjects[id];
            obj.src = srcIt->second.getString();
            auto changesIt = objElem.find(CHANGES_TAG);
            if (changesIt != objElem.end())
            {
                parseModifications(changesIt->second, obj.overrides);
            }
        }
    }

    // Check root ID
    if (m_rootID != NO_ROOT)
    {
        if (m_objects.find(m_rootID) == m_objects.end() &&
            m_overridedObjects.find(m_rootID) == m_overridedObjects.end())
        {
            VALIDATION_ERROR("Invalid root ID (" << m_rootID << "), object not found in file.");
            m_rootID = NO_ROOT;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
// Static
bool ObjectDB::getRef(const Variant & o, u32 & out_ref)
{
    if (o.isDictionary())
    {
        const Variant::Dictionary & obj = o.getDictionary();
        auto it = obj.find(REF_TAG);
        if (it != obj.end())
        {
            const Variant & v = it->second;
            if (v.isInt())
            {
                out_ref = v.getInt();
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------
void ObjectDB::parseModifications(const Variant & o, std::vector<ObjectDB::Modification> & out_modifications)
{
    size_t elemCount = o.getArray().size();
    if (elemCount % 3 != 0)
    {
        VALIDATION_ERROR("Change array hasn't a size multiple of 3. Some changes will not be applied.");
        elemCount = 3 * (elemCount / 3);
    }
    
    size_t modifCount = elemCount / 3;
    out_modifications.reserve(out_modifications.size() + modifCount);

    for (size_t i = 0; i < elemCount; i += 3)
    {
        Modification modif;

        // Get target ID
        modif.target = o[i].getInt();

        // Get path
        parsePath(o[i + 1], modif.path);

        // Get value
        modif.value = o[i + 2];

        out_modifications.push_back(modif);
    }
}

//------------------------------------------------------------------------------
void ObjectDB::parsePath(const Variant & o, std::vector<std::string> & out_path)
{
    if (o.getType() == SN_VT_STRING)
    {
        const std::string & s = o.getString();
        out_path.push_back(s);
    }
    else if (o.getType() == SN_VT_ARRAY)
    {
        const Variant::Array & a = o.getArray();
        out_path.reserve(out_path.size() + a.size());
        for (size_t i = 0; i < a.size(); ++i)
        {
            const std::string & elem = a[i].getString();
            if (elem.empty())
                SN_ERROR("A part of the path is empty!");
            out_path.push_back(elem);
        }
    }
}

//------------------------------------------------------------------------------
void ObjectDB::clear()
{
    m_objects.clear();
    m_overridedObjects.clear();
    m_nextID = 0;
    m_rootID = NO_ROOT;
    m_isFlattened = false;
}

//------------------------------------------------------------------------------
void ObjectDB::clearInstances()
{
    for (auto it = m_overridedObjects.begin(); it != m_overridedObjects.end(); ++it)
    {
        u32 id = it->first;
        m_objects.erase(id);
    }
}

//------------------------------------------------------------------------------
bool ObjectDB::validate(Variant & a_doc, u32 * out_nextID)
{
    if (!a_doc.isDictionary())
    {
        VALIDATION_ERROR("Expected object type");
        return false;
    }

    // Check format string
    const Variant & formatElem = a_doc[FORMAT_KEY];
    if (!formatElem.isString())
    {
        VALIDATION_ERROR("Expected string for field " << FORMAT_KEY);
        return false;
    }
	if (formatElem.getString() != FORMAT_VALUE)
	{
		SN_ERROR("Invalid JSON format, expected " << FORMAT_VALUE);
		return false;
	}

    // Check objects array
	const Variant & objectsElem = a_doc[OBJECTS_TAG];
	if (!objectsElem.isArray())
	{
		VALIDATION_ERROR("expected array in " << OBJECTS_TAG);
		return false;
	}

    const Variant::Array & objectsArray = objectsElem.getArray();
    size_t maxID = 0;
    std::unordered_set<u32> ids;

    if (objectsArray.size() % 2 != 0)
    {
        VALIDATION_ERROR("Expected even number of elements in " << OBJECTS_TAG << " array, found " << objectsArray.size());
        return false;
    }

    // Check objects
    for (size_t i = 0; i < objectsArray.size(); ++i)
    {
        const Variant & elem = objectsArray[i];

        bool isID = i % 2 == 0;

        if (isID)
        {
            if (!elem.isInt())
            {
                VALIDATION_ERROR("expected integer ID");
                return false;
            }
            else
            {
                s32 sid = elem.getInt();
                if (sid < 0)
                {
                    VALIDATION_ERROR("expected positive ID, got " << sid);
                    return false;
                }
                u32 id = static_cast<u32>(sid);
                if (ids.find(id) != ids.end())
                {
                    VALIDATION_ERROR("duplicate ID found: " << id);
                    return false;
                }
                ids.insert(id);
                if (id > maxID)
                    maxID = id;
            }
        }
        else
        {
            if (!elem.isDictionary())
            {
                VALIDATION_ERROR("expected object");
                return false;
            }
        }
    }
    
    // Check nextID
    u32 calculatedNextID = maxID + 1;
    Variant & nextIDElem = a_doc[NEXT_ID_TAG];
    if (nextIDElem.isInt())
    {
        if (nextIDElem.getInt() < 0 || static_cast<u32>(nextIDElem.getInt()) <= maxID)
        {
            VALIDATION_WARNING("Invalid " << NEXT_ID_TAG << " (" << nextIDElem.getInt() << "), should be at least " << calculatedNextID);
            SN_MORE("The value will be replaced.");
            nextIDElem.setInt(calculatedNextID);
        }
    }
    else
    {
        VALIDATION_WARNING(NEXT_ID_TAG << " integer not found, it will be generated (" << calculatedNextID << ")");
        nextIDElem.setInt(calculatedNextID);
    }
    if (out_nextID)
        *out_nextID = nextIDElem.getInt();

	return true;
}

//------------------------------------------------------------------------------
const Variant * ObjectDB::getObject(u32 id) const
{
    auto it = m_objects.find(id);
    if (it == m_objects.end())
        return nullptr;
    return &it->second.data;
}

//------------------------------------------------------------------------------
Variant * ObjectDB::getObject(u32 index)
{
    auto it = m_objects.find(index);
    if (it == m_objects.end())
        return nullptr;
    return &it->second.data;
}

//------------------------------------------------------------------------------
const Variant * ObjectDB::getRootObject() const
{
    if (m_rootID == NO_ROOT)
        return nullptr;
    return getObject(m_rootID);
}

//------------------------------------------------------------------------------
bool ObjectDB::isFlattened()
{
    return m_isFlattened;
}

//------------------------------------------------------------------------------
void ObjectDB::flatten()
{
	std::vector<ObjectDB*> stack;
	flatten(stack);
}

//------------------------------------------------------------------------------
u32 ObjectDB::makeID()
{
    return m_nextID++;
}

//------------------------------------------------------------------------------
void ObjectDB::flatten(std::vector<ObjectDB*> & stack)
{
	// Check for prefab cycles
	for (auto it = stack.begin(); it != stack.end(); ++it)
	{
		if (*it == this)
		{
			SN_ERROR("ObjectDB prefab cycle detected!");
			// TODO Print chain
			return;
		}
	}

    m_isFlattened = false;

    for (auto ooIt = m_overridedObjects.begin(); ooIt != m_overridedObjects.end(); ++ooIt)
    {
        u32 id = ooIt->first;
        OverrideObject & overrideObj = ooIt->second;
        if (!flattenObject(overrideObj, id, stack))
            continue;
    }

    m_isFlattened = true;
}

//------------------------------------------------------------------------------
ObjectDB * ObjectDB::getFromAssetDatabase(const std::string & location) const
{
    // Default implementation
    AssetLocation loc(location);
    if (loc.module.empty())
    {
        loc.module = getAssetMetadata().module;
    }
    return AssetDatabase::get().getAsset<ObjectDB>(loc);
}

//------------------------------------------------------------------------------
bool ObjectDB::flattenObject(OverrideObject & overrideObj, u32 id, std::vector<ObjectDB*> & stack)
{
    // Get the source file we are going to instantiate
    ObjectDB * src = getFromAssetDatabase(overrideObj.src);
    if (src == nullptr)
    {
        SN_ERROR("Cannot instantiate data object, source object '" << overrideObj.src << "' not found");
        return false;
    }

    // Ensure it's flattened
    if (!src->isFlattened())
    {
		stack.push_back(this);
		src->flatten(stack);
		stack.pop_back();
    }

    // Get its root object
    const Variant * srcObj = src->getRootObject();
    if (srcObj == nullptr)
    {
        SN_ERROR("No root object found in source '" << overrideObj.src << "', cannot instantiate data.");
        return false;
    }

    // Instantiate objects
    std::unordered_map<u32, u32> srcToInstanceID;
    const std::unordered_map<u32, Object> & srcObjects = src->m_objects;
    for (auto it = srcObjects.begin(); it != srcObjects.end(); ++it)
    {
        // Note: only the override object will have the same ID as its instance.

        u32 instanceID = it->first == src->m_rootID ? id : makeID();
        Object & obj = m_objects[instanceID];
        obj.data = it->second.data;

        srcToInstanceID[it->first] = instanceID;
    }

    // Instantiate references
    for (auto it = srcToInstanceID.begin(); it != srcToInstanceID.end(); ++it)
    {
        Object & obj = m_objects[it->second];
        mapReferences(obj.data, srcToInstanceID);
    }

    // Apply changes (overrides)
    for (auto it = overrideObj.overrides.begin(); it != overrideObj.overrides.end(); ++it)
    {
        const Modification & change = *it;
            
        u32 targetID = srcToInstanceID[targetID];
        Variant * targetObj = getObject(targetID);
        if (targetObj == nullptr)
        {
            SN_ERROR("Cannot apply override change: target object not found (in src: " << targetID << ")");
            SN_MORE("Instantiating '" << overrideObj.src << "'");
            continue;
        }

        applyChange(*targetObj, change);
    }

    return true;
}

//------------------------------------------------------------------------------
void ObjectDB::applyChange(Variant & obj, Modification change)
{
    // Get or create value
    Variant * tobj = &obj;
    for (size_t i = 0; i < change.path.size(); ++i)
    {
        const std::string & pathElem = change.path[i];
        tobj = &(*tobj)[pathElem];
    }

    // Set value
    *tobj = change.value;
}

//------------------------------------------------------------------------------
void ObjectDB::mapReferences(Variant & obj, std::unordered_map<u32, u32> refMap)
{
    if (obj.isDictionary())
    {
        // For each member
        const Variant::Dictionary objMap = obj.getDictionary();
        for (auto it = objMap.begin(); it != objMap.end(); ++it)
        {
            Variant & elem = obj[it->first];
            mapReference(elem, refMap);
        }
    }
    else if (obj.isArray())
    {
        // For each item
        Variant::Array & a = obj.getArray();
        for (size_t i = 0; i < a.size(); ++i)
        {
            Variant & elem = a[i];
            mapReference(elem, refMap);
        }
    }
}

//------------------------------------------------------------------------------
void ObjectDB::mapReference(Variant & obj, std::unordered_map<u32, u32> refMap)
{
    // If the value is an object
    if (obj.isDictionary())
    {
        const Variant::Dictionary & v = obj.getDictionary();
        auto tagIt = v.find(REF_TAG);

        // If the value represents a reference
        if (tagIt != v.end())
        {
            // Map the reference
            auto refIt = refMap.find(tagIt->second.getInt());
            if (refIt == refMap.end())
            {
                SN_ERROR("Cannot instantiate reference, not found in map");
                return;
            }
            u32 newID = refIt->second;

            obj[REF_TAG].setInt(newID);
        }
        else
        {
            // Go recursively
            mapReferences(obj, refMap);
        }
    }
}

} // namespace sn

