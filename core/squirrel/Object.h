#ifndef __HEADER_SQUIRREL_OBJECT__
#define __HEADER_SQUIRREL_OBJECT__

#include <core/squirrel/bind_tools.h>

namespace squirrel
{

/// \brief Base class holding a reference to a Squirrel object.
/// C++ takes shared ownership on this instance.
class SN_API Object
{
public:
    /// \brief Used by default in the nparams argument of function binding.
    /// Negative values have a meaning, so this special constant has a big negative value.
    /// (It would mean functions are valid if called with at least 65536 arguments, which is far away from reality)
    static const SQInteger NO_PARAMCHECK = -65536;

    /// \brief Constructs a null reference
    Object(HSQUIRRELVM vm=nullptr);
    /// \brief Constructs a reference to the given object
    Object(HSQUIRRELVM vm, HSQOBJECT obj);
    /// \brief Constructs an additional reference from an existing one
    Object(const Object & other);

    ~Object();

    HSQOBJECT getObject() const { return m_object; }
    HSQUIRRELVM getVM() const { return m_vm; }

    bool isNull() const;
    void releaseObject();

    /// \brief Sets a native method callable from scripts. This only works if the object is a key/value container.
	/// \param methodName: name of the method in scripts
	/// \param cb_method: C-style Squirrel function to execute
	/// \param nparams: (optional) fixes the number of parameter required by the function. 
	///                 Negative value -X means "at least X params". 
	///                 0 means no param checking.
	/// \param a_paramsMask: (optional) parameters type mask for checking.
	///                 For syntax details see http://squirrel-lang.org/doc/squirrel3.html#sq_setparamscheck
	/// \return Object itself for chaining
    Object & setMethod(
        const char * methodName, 
        SQFUNCTION cb_method, 
        SQInteger nparams=NO_PARAMCHECK, 
        const std::string & a_paramsMask="",
        SQBool isStatic = false
    );

    Object & operator=(const Object & other);

protected:
    HSQOBJECT m_object;
    HSQUIRRELVM m_vm;
};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_OBJECT__

