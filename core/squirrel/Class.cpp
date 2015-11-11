#include "Class.h"
#include <core/util/assert.h>

namespace squirrel
{

//------------------------------------------------------------------------------
static const char * CLASSES_TABLE = "__classes";

//------------------------------------------------------------------------------
namespace
{
    SQRESULT cb_privateConstructor(HSQUIRRELVM vm)
    {
        return sq_throwerror(vm, "Cannot instantiate this class, constructor is private.");
    }
}

//------------------------------------------------------------------------------
Class::Class(HSQUIRRELVM vm, const std::string & className, const std::string & baseClassName) : Object(vm)
{
    pushClassesTable();

    sq_pushstring(vm, className.c_str(), -1);

    // Get the class
    if (SQ_FAILED(sq_rawget(vm, -2)))
    {
        // If it doesn't exists, create it

        // Push the base if specified
        SQBool hasBase = !baseClassName.empty();
        if (hasBase)
        {
            sq_pushstring(vm, baseClassName.c_str(), -1);
            if (SQ_FAILED(sq_rawget(vm, -2)))
            {
                SN_ERROR("Base class not found: '" << baseClassName << "' when binding class '" << className << "'");
            }
        }

#ifdef SN_BUILD_DEBUG
        SN_LOG("Creating Squirrel class '" << className << "'");
#endif

        // Create the class (the inherited class will be popped if specified)
        sq_newclass(vm, hasBase);

        // Store it in the classes table
        sq_pushstring(vm, className.c_str(), -1);
        sq_push(vm, -2); // The class will be on the stack after that
        sq_rawset(vm, -4);
    }
    else
    {
        SN_WARNING("Overriding Squirrel class '" << className << "'");
    }

    // Reference the class object in C++ for further access
    sq_getstackobj(vm, -1, &m_object);
    sq_addref(vm, &m_object);

    // Pop class, classes table and registry table
    sq_pop(vm, 3);
}

//------------------------------------------------------------------------------
Class & Class::setConstructor(SQFUNCTION cb_constructor)
{
    return setMethod("constructor", cb_constructor);
}

//------------------------------------------------------------------------------
Class & Class::setPrivateConstructor()
{
    return setConstructor(cb_privateConstructor);
}

//------------------------------------------------------------------------------
Class & Class::setMethod(const char * methodName, SQFUNCTION cb_method, SQInteger nparams, const std::string & a_paramsMask, SQBool isStatic)
{
    SN_ASSERT(!isNull(), "Class is null");
    SN_ASSERT(cb_method != nullptr, "Function pointer argument is null");

    sq_pushobject(m_vm, m_object);
    sq_pushstring(m_vm, methodName, -1);
    sq_newclosure(m_vm, cb_method, 0);

    if (nparams != NO_PARAMCHECK)
    {
        // Note: we need to include the "this" parameter (a class if static, an instance otherwise)
        std::string paramsMask = (isStatic ? "y" : "x") + a_paramsMask;
        sq_setparamscheck(m_vm, nparams+1, paramsMask.c_str());
    }

    // Store the method
    sq_newslot(m_vm, -3, isStatic);

    sq_pop(m_vm, 1);

    return *this;
}

//------------------------------------------------------------------------------
//Class & Class::setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter)
//{
//    SN_ASSERT(!isNull(), "Class is null");
//    SN_ASSERT(cb_getter != nullptr || cb_setter != nullptr, "Property function pointers are both null");
//
//    // TODO
//
//    return *this;
//}

//------------------------------------------------------------------------------
void Class::pushClassesTable()
{
    auto vm = m_vm;
    // Get classes table
    sq_pushregistrytable(vm);
    sq_pushstring(vm, CLASSES_TABLE, -1);
    if (SQ_FAILED(sq_rawget(vm, -2)))
    {
        // If it doesn't exist, create it (still on the stack after the rawset)
        sq_newtable(vm);
        // Store the table in the classes table
        sq_pushstring(vm, CLASSES_TABLE, -1);
        sq_push(vm, -2);
        sq_rawset(vm, -4);
    }

}

} // namespace squirrel

