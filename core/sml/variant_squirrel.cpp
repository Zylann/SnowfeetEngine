#include "variant_squirrel.h"

namespace sn
{

//------------------------------------------------------------------------------
void pushValue(HSQUIRRELVM vm, const Variant & v)
{
	switch (v.getType().id)
	{
	case SN_VT_STRING:
		{
			const std::string & s = v.getString();
			sq_pushstring(vm, s.c_str(), s.size());
		}
		break;

	case SN_VT_INT:
		sq_pushinteger(vm, v.getInt());
		break;

	case SN_VT_FLOAT:
		sq_pushfloat(vm, static_cast<float>(v.getFloat()));
		break;

	case SN_VT_DICTIONARY:
		// TODO Handle objects
		// TODO Handle special objects (color, vectors, matrix, instances etc)
		break;

	case SN_VT_ARRAY:
		// TODO Handle arrays
		break;

	case SN_VT_BOOL:
		sq_pushbool(vm, v.getBool());
		break;

	case SN_VT_NIL:
		sq_pushnull(vm);
		break;

	default:
		// UNKNOWN
		break;
	}
}

//------------------------------------------------------------------------------
void applyProperties(HSQUIRRELVM vm, const Variant & o, SQInteger objectIndex)
{
	SN_ASSERT(o.isDictionary(), "Variant is not a dictionary");

	const Variant::Dictionary & obj = o.getDictionary();
	for (auto it = obj.begin(); it != obj.end(); ++it)
	{
		// Push key
		const std::string & key = it->first;
		sq_pushstring(vm, key.c_str(), key.size());

		// Push value
		const Variant & value = it->second;
		pushValue(vm, value);

		// Set
		if (SQ_FAILED(sq_set(vm, objectIndex)))
		{
			SN_WARNING("Variant => Squirrel: failed to set key " << key);
		}
	}
}

} // namespace sn

