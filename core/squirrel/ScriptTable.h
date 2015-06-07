#ifndef __HEADER_SN_SCRIPTTABLE__
#define __HEADER_SN_SCRIPTTABLE__

#include <core/squirrel/ScriptObject.h>

namespace sn
{

/// \brief Base class for manipulating a Squirrel table
class SN_API ScriptTable : public ScriptObject
{
public:
    ScriptTable(HSQUIRRELVM vm);

    ScriptTable & setFunction(const char * name, SQFUNCTION cb_func);
    ScriptTable & setObject(const char * name, ScriptObject & obj);

};

/// \brief Root table where all globals are stored
class SN_API ScriptRootTable : public ScriptTable
{
public:
    ScriptRootTable(HSQUIRRELVM vm);
};

/// \brief Special table where non-Squirrel objects can be stored without conflicting with scripts
class SN_API ScriptRegistryTable : public ScriptTable
{
public:
    ScriptRegistryTable(HSQUIRRELVM vm);
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTTABLE__

