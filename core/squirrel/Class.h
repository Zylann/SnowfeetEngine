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
    /// \brief Gets or creates a Squirrel class. It will be stored in the registry table.
    /// To make it accessible from the roottable, you have to store it there too (This is to prevent scripters from messing with the class)
    /// \param vm
    /// \param className: name of the class in scripts
    /// \param baseClassName: optional name of the base class to inherit if the class is created
    Class(HSQUIRRELVM vm, const std::string & className, const std::string & baseClassName="");

    /// \brief Sets the native function that will be called when the Squirrel class is instantiated.
    /// This is only relevant for classes, but also works on tables.
    Class & setConstructor(
        SQFUNCTION cb_constructor,
        SQInteger nparams=NO_PARAMCHECK,
        const std::string & a_paramsMask=""
    );

    Class & setPrivateConstructor();

    //TODO Class & setProperty(const char * propertyName, SQFUNCTION cb_getter, SQFUNCTION cb_setter=nullptr, const std::string & a_paramMask);

private:
    static void pushClassesTable(HSQUIRRELVM vm);

};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_CLASS__

