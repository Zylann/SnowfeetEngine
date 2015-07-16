#include "PackedObject.h"
#include "../util/Log.h"
#include "../asset/AssetDatabase.h"

namespace sn
{

//------------------------------------------------------------------------------
const char * PackedObject::FORMAT_STRING  = "SN2";
const char * PackedObject::XREF_TAG       = "@xref";
const char * PackedObject::PREFAB_TAG     = "@prefab";
const char * PackedObject::OBJECTS_TAG    = "objects";

const char * PackedObject::TYPE_TAG       = "@type";
const char * PackedObject::REF_TAG        = "@ref";

//------------------------------------------------------------------------------
bool PackedObject::loadFromStream(std::istream & is)
{
	m_jsonData.loadFromStream(is);

	// Validate
	if (validate())
	{
		flatten();
		return true;
	}
	else
	{
		m_jsonData.setNull();
		return false;
	}
}

//------------------------------------------------------------------------------
bool PackedObject::validate()
{
	if (m_jsonData["format"] != FORMAT_STRING)
	{
		SN_ERROR("Invalid JSON format, expected " << FORMAT_STRING);
		return false;
	}

	JsonBox::Value & objects = m_jsonData[OBJECTS_TAG];
	if (!objects.isArray())
	{
		SN_ERROR("Invalid " << FORMAT_STRING << " format: expected array of objects");
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
JsonBox::Value & PackedObject::getObject(size_t index)
{
	JsonBox::Value & v = m_flattenedJsonData.isNull() ? m_jsonData : m_flattenedJsonData;
	return v[OBJECTS_TAG][index];
}

//------------------------------------------------------------------------------
bool PackedObject::isFlattened()
{
	return m_flattenedJsonData.isNull();
}

//------------------------------------------------------------------------------
void PackedObject::flatten()
{
	std::vector<PackedObject*> stack;
	flatten(stack);
}

//------------------------------------------------------------------------------
void PackedObject::flatten(std::vector<PackedObject*> & stack)
{
	// Check for prefab cycles
	for (auto it = stack.begin(); it != stack.end(); ++it)
	{
		if (*it == this)
		{
			SN_ERROR("PackedObject prefab cycle detected!");
			// TODO Print chain
			return;
		}
	}

	m_flattenedJsonData = m_jsonData;
	JsonBox::Value & objects = m_flattenedJsonData[OBJECTS_TAG];

	size_t objectCount = objects.getArray().size();

	for (size_t i = 0; i < objectCount; ++i)
	{
		JsonBox::Value & obj = objects[i];

		if (!obj.isObject())
			continue;

		// Process prefabs
		JsonBox::Value & prefabField = obj[PREFAB_TAG];
		if (prefabField.isString())
		{
			size_t xref = 0;
			JsonBox::Value & xrefField = obj[XREF_TAG];
			if (xrefField.isInteger())
			{
				xref = xrefField.getInt();
			}

			const std::string location = prefabField.getString();
			PackedObject * prefab = AssetDatabase::get().getAsset<PackedObject>(AssetLocation(location));
			if (!prefab->isFlattened())
			{
				stack.push_back(this);
				prefab->flatten(stack);
				stack.pop_back();
			}

			JsonBox::Value & otherObj = prefab->getObject(xref);
			if (otherObj.isObject())
			{
				const JsonBox::Object & overrideMap = obj.getObject();
				const JsonBox::Object & baseMap = otherObj.getObject();

				for (auto it = baseMap.begin(); it != baseMap.end(); ++it)
				{
					if (overrideMap.find(it->first) == overrideMap.end())
					{
						obj[it->first] = it->second;
					}
				}
			}
		}
	}
}

} // namespace sn

