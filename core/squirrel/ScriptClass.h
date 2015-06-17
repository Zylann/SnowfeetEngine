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
    /// \brief Used by default in the nparams argument of function binding.
    /// Negative values have a meaning, so this special constant has a big negative value.
    /// (It would mean functions are valid if called with at least 65536 arguments, which is far away from reality)
    static const s32 NO_PARAMCHECK = -65536;

    /// \brief Gets or creates a Squirrel class.
    /// \param vm
    /// \param className: name of the class in scripts
    /// \param baseClassName: optional name of the base class to inherit if the class is created
    ScriptClass(HSQUIRRELVM vm, const std::string & className, const std::string & baseClassName="");

    /// \brief Sets the native function that will be called when the Squirrel class is instantiated.
    ScriptClass & setConstructor(SQFUNCTION cb_constructor);

    //ScriptClass & setRelease(SQFUNCTION cb_release);

    /// \brief Sets a native method callable from scripts
    ScriptClass & setMethod(const char * methodName, SQFUNCTION cb_method, s32 nparams=NO_PARAMCHECK, const char * paramsMask=nullptr);
    //ScriptClass & setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter=nullptr);

private:
    void pushClassesTable();

};

} // namespace sn

#endif // __HEADER_SN_SCRIPTCLASS__

