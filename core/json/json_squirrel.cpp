#include "json_squirrel.h"
#include <core/util/assert.hpp>

namespace sn
{

//------------------------------------------------------------------------------
void pushValue(HSQUIRRELVM vm, const JsonBox::Value & v)
{
	switch (v.getType())
	{
	case JsonBox::Value::STRING:
		{
			const std::string & s = v.getString();
			sq_pushstring(vm, s.c_str(), s.size());
		}
		break;

	case JsonBox::Value::INTEGER:
		sq_pushinteger(vm, v.getInt());
		break;

	case JsonBox::Value::DOUBLE:
		sq_pushfloat(vm, static_cast<float>(v.getDouble()));
		break;

	case JsonBox::Value::OBJECT:
		// TODO Handle objects
		// TODO Handle special objects (color, vectors, matrix, instances etc)
		break;

	case JsonBox::Value::ARRAY:
		// TODO Handle arrays
		break;

	case JsonBox::Value::BOOLEAN:
		sq_pushbool(vm, v.getBoolean());
		break;

	case JsonBox::Value::NULL_VALUE:
		sq_pushnull(vm);
		break;

	default:
		// UNKNOWN
		break;
	}
}

//------------------------------------------------------------------------------
void applyProperties(HSQUIRRELVM vm, const JsonBox::Value & o, SQInteger objectIndex)
{
	SN_ASSERT(o.isObject(), "Json is not an object");

	const JsonBox::Object & obj = o.getObject();
	for (auto it = obj.begin(); it != obj.end(); ++it)
	{
		// Push key
		const std::string & key = it->first;
		sq_pushstring(vm, key.c_str(), key.size());

		// Push value
		const JsonBox::Value & value = it->second;
		pushValue(vm, value);

		// Set
		if (SQ_FAILED(sq_set(vm, objectIndex)))
		{
			SN_WARNING("Json => Squirrel: failed to set key " << key);
		}
	}
}

} // namespace sn

