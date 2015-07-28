#ifndef __HEADER_SQUIRREL_TABLE__
#define __HEADER_SQUIRREL_TABLE__

#include <core/squirrel/Object.h>
#include <core/squirrel/push.h>
#include <core/util/assert.h>

namespace squirrel
{

/// \brief Base class for manipulating a Squirrel table
class SN_API Table : public Object
{
public:
	Table() : Object() {}
    Table(HSQUIRRELVM vm, HSQOBJECT obj) : Object(vm, obj) {}

	void create(HSQUIRRELVM vm);

	// TODO Replace them by the new template version
    Table & setFunction(const char * name, SQFUNCTION cb_func);
    Table & setObject(const char * name, HSQOBJECT obj);
    Table & setObject(const char * name, Object & obj);

	/// \brief Sets or creates a key/value pair in the table. The table must exist.
	/// \param key
	/// \param value
	/// \return Object instance for call chaining.
	template <typename Key_T, typename Value_T>
	Table & setField(Key_T key, Value_T value)
	{
		SN_ASSERT(!isNull(), "Table is null");
		sq_pushobject(m_vm, m_object);
		squirrel::push(m_vm, key);
		squirrel::push(m_vm, value);
		sq_newslot(m_vm, -3, SQFalse);
		sq_pop(m_vm, 1);
		return *this;
	}

	bool getString(HSQOBJECT keyObject, std::string & out_str);

protected:
    Table(HSQUIRRELVM vm) : Object(vm) {}

};

/// \brief Root table where all globals are stored
class SN_API RootTable : public Table
{
public:
    RootTable(HSQUIRRELVM vm);
};

/// \brief Special table where non-Squirrel objects can be stored without conflicting with scripts
class SN_API RegistryTable : public Table
{
public:
    RegistryTable(HSQUIRRELVM vm);
};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_TABLE__

