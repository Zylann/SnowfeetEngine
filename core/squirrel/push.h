#ifndef __HEADER_SQUIRREL_PUSH__
#define __HEADER_SQUIRREL_PUSH__

#include <core/squirrel/Object.h>

namespace squirrel
{

inline void push(HSQUIRRELVM vm, bool v)
{
    sq_pushbool(vm, v);
}

inline void push(HSQUIRRELVM vm, SQInteger v)
{
    sq_pushinteger(vm, v);
}

inline void push(HSQUIRRELVM vm, SQUnsignedInteger v)
{
	sq_pushinteger(vm, v);
}

inline void push(HSQUIRRELVM vm, SQFloat v)
{
    sq_pushfloat(vm, v);
}

inline void push(HSQUIRRELVM vm, const SQChar * str)
{
    sq_pushstring(vm, str, -1);
}

inline void push(HSQUIRRELVM vm, const std::string & str)
{
    sq_pushstring(vm, str.c_str(), str.size());
}

inline void push(HSQUIRRELVM vm, HSQOBJECT obj)
{
	sq_pushobject(vm, obj);
}

inline void push(HSQUIRRELVM vm, const Object & obj)
{
	sq_pushobject(vm, obj.getObject());
}

inline void push(HSQUIRRELVM vm, SQFUNCTION f)
{
	sq_newclosure(vm, f, 0);
}

} // namespace squirrel

#endif // __HEADER_SQUIRREL_PUSH__

