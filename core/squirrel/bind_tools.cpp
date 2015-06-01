#include <iostream>
#include "bind_tools.hpp"

namespace sn
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

} // namespace sn



