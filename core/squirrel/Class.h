#ifndef __HEADER_SQUIRREL_CLASS__
#define __HEADER_SQUIRREL_CLASS__

#include <core/squirrel/Object.h>
#include <string>

namespace squirrel
{

/// \brief Wraps a script class that can be used to bind C++ classes.
class SN_API Class : public Object
{
public:
    /// \brief Used by default in the nparams argument of function binding.
    /// Negative values have a meaning, so this special constant has a big negative value.
    /// (It would mean functions are valid if called with at least 65536 arguments, which is far away from reality)
    static const SQInteger NO_PARAMCHECK = -65536;

    /// \brief Gets or creates a Squirrel class. It will be stored in the registry table.
    /// To make it accessible from the roottable, you have to store it there too (This is to prevent scripters from messing with the class)
    /// \param vm
    /// \param className: name of the class in scripts
    /// \param baseClassName: optional name of the base class to inherit if the class is created
    Class(HSQUIRRELVM vm, const std::string & className, const std::string & baseClassName="");

    /// \brief Sets the native function that will be called when the Squirrel class is instantiated.
    Class & setConstructor(SQFUNCTION cb_constructor);

    Class & setPrivateConstructor();

    /// \brief Sets a native method callable from scripts
	/// \param methodName: name of the method in scripts
	/// \param cb_method: C-style Squirrel function to execute
	/// \param nparams: (optional) fixes the number of parameter required by the function. 
	///                 Negative value -X means "at least X params". 
	///                 0 means no param checking.
	/// \param a_paramsMask: (optional) parameters type mask for checking.
	///                 For syntax details see http://squirrel-lang.org/doc/squirrel3.html#sq_setparamscheck
	/// \return Object itself for chaining
    Class & setMethod(
        const char * methodName, 
        SQFUNCTION cb_method, 
        SQInteger nparams=NO_PARAMCHECK, 
        const std::string & a_paramsMask="",
        SQBool isStatic = false
    );

    //TODO Class & setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter=nullptr, const std::string & a_paramMask);

private:
    void pushClassesTable();

};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_CLASS__

