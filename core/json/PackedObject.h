#ifndef __HEADER_SN_PACKEDOBJECT__
#define __HEADER_SN_PACKEDOBJECT__

#include <iostream>
#include <JsonBox.h>

namespace sn
{

/// \brief Represents the serialized state of one or a collection of objects.
class PackedObject
{
public:
	static const char * FORMAT_STRING;
	static const char * XREF_TAG;
	static const char * PREFAB_TAG;
	static const char * OBJECTS_TAG;

	static const char * TYPE_TAG;
	static const char * REF_TAG;

	bool loadFromStream(std::istream & is);

	JsonBox::Value & getObject(size_t index);

private:
	bool validate();
	bool isFlattened();
	void flatten();
	void flatten(std::vector<PackedObject*> & stack);

private:
	JsonBox::Value m_jsonData;
	JsonBox::Value m_flattenedJsonData;
};

} // namespace sn

#endif // __HEADER_SN_PACKEDOBJECT__

