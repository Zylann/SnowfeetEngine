#include <iostream>
#include "bind_tools.h"
#include <core/util/Log.h>

namespace squirrel
{

void debugStackDump(HSQUIRRELVM v)
{
    SN_LOG("Squirrel stack dump:");
    for (SQInteger n = 1; n <= sq_gettop(v); ++n)
    {
        std::string s = "[" + std::to_string(n) + "] ";
        s += squirrel::getTypeName(sq_gettype(v, n));
        SN_MORE(s);
    }
}

const char * getTypeName(SQInteger type)
{
    switch (type)
    {
    case OT_NULL:           return "null";
    case OT_INTEGER:        return "integer";
    case OT_FLOAT:          return "float";
    case OT_STRING:         return "string";
    case OT_TABLE:          return "table";
    case OT_ARRAY:          return "array";
    case OT_USERDATA:       return "userdata";
    case OT_CLOSURE:        return "closure(function)";
    case OT_NATIVECLOSURE:  return "native closure(C function)";
    case OT_GENERATOR:      return "generator";
    case OT_USERPOINTER:    return "userpointer";
    case OT_CLASS:          return "class";
    case OT_INSTANCE:       return "instance";
    case OT_WEAKREF:        return "weak reference";
    default: break;
    }
    return "unknown";
}

void setGlobalFunction(HSQUIRRELVM vm, const char * name, SQFUNCTION cb_func)
{
    sq_pushroottable(vm);
    sq_pushstring(vm, name, -1);
    sq_newclosure(vm, cb_func, 0);
    sq_newslot(vm, -3, false);
    sq_pop(vm, 1); // pop roottable
}

std::string getLastError(HSQUIRRELVM vm)
{
    const SQChar* errorString = nullptr;

    sq_getlasterror(vm);
    if (sq_gettype(vm, -1) == OT_NULL)
    {
		sq_pop(vm, 1);
        return std::string();
    }

    sq_tostring(vm, -1);
    sq_getstring(vm, -1, &errorString);
    sq_pop(vm, 2);

    return std::string(errorString);
}

} // namespace squirrel



