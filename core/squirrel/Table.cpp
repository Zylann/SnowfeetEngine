#include "Table.h"
#include <core/util/assert.hpp>

namespace squirrel
{

//------------------------------------------------------------------------------
Table & Table::setObject(const char * name, Object & obj)
{
    return setObject(name, obj.getObject());
}

//------------------------------------------------------------------------------
Table & Table::setObject(const char * name, HSQOBJECT obj)
{
    SN_ASSERT(!isNull(), "Table is null");
    SN_ASSERT(!sq_isnull(obj), "Argument object is null");

    sq_pushobject(m_vm, m_object);
    sq_pushstring(m_vm, name, -1);
    sq_pushobject(m_vm, obj);
    sq_newslot(m_vm, -3, false);
    sq_pop(m_vm, 1); // pop table

    return *this;
}

//------------------------------------------------------------------------------
Table & Table::setFunction(const char * name, SQFUNCTION cb_func)
{
    SN_ASSERT(!isNull(), "Table is null");
    SN_ASSERT(cb_func != nullptr, "Function pointer argument is null");

    sq_pushobject(m_vm, m_object);
    sq_pushstring(m_vm, name, -1);
    sq_newclosure(m_vm, cb_func, 0);
    // Store the method
    SQBool isStatic = SQFalse;
    sq_newslot(m_vm, -3, isStatic);

    sq_pop(m_vm, 1);

    return *this;
}

//------------------------------------------------------------------------------
RootTable::RootTable(HSQUIRRELVM vm) : Table(vm)
{
    sq_pushroottable(vm);
    sq_getstackobj(vm, -1, &m_object);
    sq_addref(vm, &m_object);
    sq_pop(vm, 1);
}

//------------------------------------------------------------------------------
RegistryTable::RegistryTable(HSQUIRRELVM vm) : Table(vm)
{
    sq_pushregistrytable(vm);
    sq_getstackobj(vm, -1, &m_object);
    sq_addref(vm, &m_object);
    sq_pop(vm, 1);
}

} // namespace squirrel

