#ifndef __HEADER_SN_SCRIPTCLASS__
#define __HEADER_SN_SCRIPTCLASS__

#include <core/squirrel/ScriptObject.h>
#include <string>

namespace sn
{

/// \brief Wraps a script class that can be used to bind C++ classes.
class SN_API ScriptClass : public ScriptObject
{
public:
    ScriptClass(HSQUIRRELVM vm, const std::string & className);

    ScriptClass & setConstructor(SQFUNCTION cb_constructor);
    //ScriptClass & setRelease(SQFUNCTION cb_release);
    ScriptClass & setMethod(const char * methodName, SQFUNCTION cb_method);
    //ScriptClass & setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter=nullptr);

private:
    void pushClassesTable();

};

} // namespace sn

#endif // __HEADER_SN_SCRIPTCLASS__

