#ifndef __HEADER_SQUIRREL_PUSH__
#define __HEADER_SQUIRREL_PUSH__

#include <squirrel.h>

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

inline void push(HSQUIRRELVM vm, SQFloat v)
{
    sq_pushfloat(vm, v);
}

inline void push(HSQUIRRELVM vm, const char * str)
{
    sq_pushstring(vm, str, -1);
}

inline void push(HSQUIRRELVM vm, const std::string & str)
{
    sq_pushstring(vm, str.c_str(), str.size());
}

} // namespace squirrel

#endif // __HEADER_SQUIRREL_PUSH__
