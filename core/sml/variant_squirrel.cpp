#include "variant_squirrel.h"
#include <core/squirrel/bind_tools.h>

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
		pushTable(vm, v.getDictionary());
		break;

	case SN_VT_ARRAY:
		pushArray(vm, v.getArray());
		break;

	case SN_VT_BOOL:
		sq_pushbool(vm, v.getBool());
		break;

	case SN_VT_NIL:
		sq_pushnull(vm);
		break;

	default:
		// TODO Handle special objects (color, vectors, matrix, instances etc)
		// UNKNOWN
		break;
	}
}

//------------------------------------------------------------------------------
void SN_API pushTable(HSQUIRRELVM vm, const Variant::Dictionary & d)
{
	sq_newtable(vm);
	for (auto it = d.begin(); it != d.end(); ++it)
	{
		// Push key
		const std::string & key = it->first;
		sq_pushstring(vm, key.c_str(), key.size());

		// Push value
		const Variant & value = it->second;
		pushValue(vm, value);

		// Set or create
		if (SQ_FAILED(sq_newslot(vm, -3, SQFalse)))
		{
			SN_WARNING("Variant => Squirrel: failed to set key " << key);
		}
	}
}

//------------------------------------------------------------------------------
void SN_API pushArray(HSQUIRRELVM vm, const Variant::Array & a)
{
	sq_newarray(vm, a.size());
	for (size_t i = 0; i < a.size(); ++i)
	{
		sq_pushinteger(vm, i);
		pushValue(vm, a[i]);
		if (SQ_FAILED(sq_set(vm, -3)))
		{
			SN_WARNING("Variant => Squirrel: failed to set array index " << i);
		}
	}
}

//------------------------------------------------------------------------------
void applyProperties(HSQUIRRELVM vm, const Variant::Dictionary & o, SQInteger objectIndex)
{
	if (objectIndex < 0)
		objectIndex -= 2; // Key + value stack offset

	for (auto it = o.begin(); it != o.end(); ++it)
	{
		// Push key
		const std::string & key = it->first;
		sq_pushstring(vm, key.c_str(), key.size());

		// Push value
		// TODO If the value is a table, go recursively instead of overwriting the previous value
		const Variant & value = it->second;
		pushValue(vm, value);

		// Set
		if (SQ_FAILED(sq_set(vm, objectIndex)))
		{
			SN_WARNING("Variant => Squirrel: failed to set key " << key);
		}
	}
}

//------------------------------------------------------------------------------
void SN_API getVariant(HSQUIRRELVM vm, Variant & out_v, SQInteger index)
{
	SQObjectType t = sq_gettype(vm, index);
	switch (t)
	{
	case OT_BOOL:
		out_v.setBool(squirrel::getBool(vm, index));
		break;

	case OT_INTEGER:
		out_v.setInt(squirrel::getInt(vm, index));
		break;

	case OT_FLOAT:
		out_v.setFloat(squirrel::getFloat(vm, index));
		break;

	case OT_STRING:
		out_v.setString(squirrel::getString(vm, index));
		break;

	case OT_ARRAY:
		getArray(vm, out_v, index);
		break;

	case OT_TABLE:
		getTable(vm, out_v, index);
		break;

	case OT_INSTANCE:
		SN_ERROR("Instance type is not supported by sn::getVariant");
		break;

	default:
		out_v.reset();
		break;
	}
}

//------------------------------------------------------------------------------
void SN_API getArray(HSQUIRRELVM vm, Variant & out_v, SQInteger index)
{
	out_v.setArray();
	Variant::Array & a = out_v.getArray();

	SQInteger len = sq_getsize(vm, index);
	SN_ASSERT(len >= 0, "Invalid array size");
	
	if (len > 0)
	{
		a.resize(len);
		s32 i = 0;

		sq_push(vm, index);
		sq_pushnull(vm);

		while (SQ_SUCCEEDED(sq_next(vm, -2)))
		{
			// -1 is the value and -2 is the key

			sq_getinteger(vm, -2, &i);
			Variant & val = a[i];
			getVariant(vm, val, -1);

			sq_pop(vm, 2); //pops key and val before the next iteration
		}

		sq_pop(vm, 2); // Pop the null iterator and array
	}
}

//------------------------------------------------------------------------------
void SN_API getTable(HSQUIRRELVM vm, Variant & out_v, SQInteger index)
{
	out_v.setDictionary();
	Variant::Dictionary & d = out_v.getDictionary();

	sq_push(vm, index);
	sq_pushnull(vm);

	while (SQ_SUCCEEDED(sq_next(vm, -2)))
	{
		if (sq_gettype(vm, -2) == OT_STRING)
		{
			const SQChar * key = nullptr;
			sq_getstring(vm, -2, &key);
			Variant & val = d[key];
			getVariant(vm, val, -1);
		}
		else
		{
			SN_WARNING("non-string keys in tables are not supported by getVariant");
		}

		sq_pop(vm, 2);
	}

	sq_pop(vm, 2); // Pop iterator and table
}

} // namespace sn

