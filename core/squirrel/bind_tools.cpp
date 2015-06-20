#include <iostream>
#include "bind_tools.hpp"
#include <core/util/Log.hpp>

namespace squirrel
{

void debugStackDump(HSQUIRRELVM v)
{
    SN_LOG("Squirrel stack dump:");
    for (SQInteger n = 1; n <= sq_gettop(v); ++n)
    {
        std::string s = "[" + std::to_string(n) + "] ";
        switch (sq_gettype(v, n))
        {
        case OT_NULL:
            s += "null";
            break;

        case OT_INTEGER:
            s += "integer";
            break;

        case OT_FLOAT:
            s += "float";
            break;

        case OT_STRING:
            s += "string";
            break;

        case OT_TABLE:
            s += "table";
            break;

        case OT_ARRAY:
            s += "array";
            break;

        case OT_USERDATA:
            s += "userdata";
            break;

        case OT_CLOSURE:
            s += "closure(function)";
            break;

        case OT_NATIVECLOSURE:
            s += "native closure(C function)";
            break;

        case OT_GENERATOR:
            s += "generator";
            break;

        case OT_USERPOINTER:
            s += "userpointer";
            break;

        case OT_CLASS:
            s += "class";
            break;

        case OT_INSTANCE:
            s += "instance";
            break;

        case OT_WEAKREF:
            s += "weak reference";
            break;

        default:
            s += "unknown";
        }

        SN_MORE(s);
    }
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



