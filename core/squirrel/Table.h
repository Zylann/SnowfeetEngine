#ifndef __HEADER_SQUIRREL_TABLE__
#define __HEADER_SQUIRREL_TABLE__

#include <core/squirrel/Object.h>

namespace squirrel
{

/// \brief Base class for manipulating a Squirrel table
class SN_API Table : public Object
{
public:
    Table(HSQUIRRELVM vm, HSQOBJECT obj) : Object(vm, obj) {}

    Table & setFunction(const char * name, SQFUNCTION cb_func);
    Table & setObject(const char * name, HSQOBJECT obj);
    Table & setObject(const char * name, Object & obj);

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

