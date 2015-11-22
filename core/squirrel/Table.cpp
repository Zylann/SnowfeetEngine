#include "Table.h"

namespace squirrel
{

//------------------------------------------------------------------------------
Table & Table::setObject(const char * name, Object & obj)
{
    return setObject(name, obj.getObject());
}

//------------------------------------------------------------------------------
void Table::create(HSQUIRRELVM vm)
{
	releaseObject();
	
	m_vm = vm;

	sq_newtable(vm);
	sq_getstackobj(vm, -1, &m_object);
	sq_addref(vm, &m_object);
	sq_pop(vm, 1);
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
bool Table::getString(HSQOBJECT keyObject, std::string & out_str)
{
	if (isNull())
		return false;
	
	auto vm = m_vm;

	sq_pushobject(vm, m_object);
	sq_pushobject(vm, keyObject);

	if (SQ_FAILED(sq_rawget(vm, -2)))
	{
		sq_pop(vm, 1);
		return false;
	}
	else
	{
		const char * str = nullptr;
		sq_getstring(vm, -1, &str);
		if (str)
		{
			out_str = str;
		}
		sq_pop(vm, 2);
		return str != nullptr;
	}
}

//------------------------------------------------------------------------------
Table & Table::setDelegate(const Table & other)
{
    SN_ASSERT(!isNull(), "Table is null");

    sq_pushobject(m_vm, m_object);
    sq_pushobject(m_vm, other.m_object);
    sq_setdelegate(m_vm, -2);
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

