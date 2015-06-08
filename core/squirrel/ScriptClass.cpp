#include "ScriptClass.h"
#include <core/util/assert.hpp>

namespace sn
{

//------------------------------------------------------------------------------
static const char * CLASSES_TABLE = "__classes";

//------------------------------------------------------------------------------
ScriptClass::ScriptClass(HSQUIRRELVM vm, const std::string & className) : ScriptObject(vm)
{
    pushClassesTable();

    sq_pushstring(vm, className.c_str(), -1);
    // Get the class
    if (SQ_FAILED(sq_rawget(vm, -2)))
    {
        // If it doesn't exists, create it
        SQBool hasBase = SQFalse;
        sq_newclass(vm, hasBase);
        // Store it in the classes table
        sq_pushstring(vm, className.c_str(), -1);
        sq_push(vm, -2); // The class will be on the stack after that
        sq_rawset(vm, -4);
    }

    sq_getstackobj(vm, -1, &m_object);
    sq_addref(vm, &m_object);

    // Pop class, classes table and registry table
    sq_pop(vm, 3);
}

//------------------------------------------------------------------------------
ScriptClass & ScriptClass::setConstructor(SQFUNCTION cb_constructor)
{
    return setMethod("constructor", cb_constructor);
}

//------------------------------------------------------------------------------
ScriptClass & ScriptClass::setMethod(const char * methodName, SQFUNCTION cb_method)
{
    SN_ASSERT(!isNull(), "Class is null");
    SN_ASSERT(cb_method != nullptr, "Function pointer argument is null");

    sq_pushobject(m_vm, m_object);
    sq_pushstring(m_vm, methodName, -1);
    sq_newclosure(m_vm, cb_method, 0);
    // Store the method
    SQBool isStatic = SQFalse;
    sq_newslot(m_vm, -3, isStatic);

    sq_pop(m_vm, 1);

    return *this;
}

//------------------------------------------------------------------------------
//ScriptClass & ScriptClass::setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter)
//{
//    SN_ASSERT(!isNull(), "Class is null");
//    SN_ASSERT(cb_getter != nullptr || cb_setter != nullptr, "Property function pointers are both null");
//
//    // TODO
//
//    return *this;
//}

//------------------------------------------------------------------------------
void ScriptClass::pushClassesTable()
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

} // namespace sn

