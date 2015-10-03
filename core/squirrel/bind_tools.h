/*
bind_tools.h
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SQUIRREL_BIND_TOOLS__
#define __HEADER_SQUIRREL_BIND_TOOLS__

#include <squirrel.h>
#include <string>
#include <core/export.h>

namespace squirrel
{

//------------------------------------------------------------------------------
/// \brief Prints the Squirrel stack in the console output.
/// This function is useful when debugging, you might be able to call it in an immediate window.
void SN_API debugStackDump(HSQUIRRELVM v);

/// \brief Gets the human-readable name of a Squirrel type
/// \param type
/// \return name
const char * getTypeName(SQInteger type);

/// \brief Binds a global function to the given Squirrel VM.
void SN_API setGlobalFunction(HSQUIRRELVM vm, const char * name, SQFUNCTION cb_func);

/// \brief Gets the last error generated in the given VM.
std::string SN_API getLastError(HSQUIRRELVM vm);

// TODO Most of the following functions are out of date, check their use and remove them

//------------------------------------------------------------------------------
/// \deprecated
/// \brief Creates a new C++ instance and assigns it as the instance userpointer
/// of the last element on the Squirrel stack.
template <typename T>
SQInteger createClassInstance(HSQUIRRELVM vm)
{
    T * p = new T();
    sq_setinstanceup(vm, -1, p);
    sq_setreleasehook(vm, -1, releaseClassInstance<T>);
    return 0;
}

//------------------------------------------------------------------------------
/// \deprecated
/// \brief (internal use) Destroys/releases a C++ instance.
/// This function is used in the Squirrel release hook.
template <typename T>
SQInteger releaseClassInstance(SQUserPointer ptr, SQInteger size)
{
    T * p = static_cast<T*>(ptr);
    priv::destroyClassInstance(p);
    return 0;
}

//------------------------------------------------------------------------------
/// \deprecated
/// \brief Gets the associated userpointer of an instance at the given index on the stack of a Squirrel VM.
/// In a custom Squirrel callback bound as a method, you can use it to retrieve your C++ instance at index 1.
template <typename T>
T * getNativeInstance(HSQUIRRELVM vm, SQInteger i)
{
    SQUserPointer p = NULL;
    sq_getinstanceup(vm, i, &p, NULL);
    return static_cast<T*>(p);
}

//------------------------------------------------------------------------------
/// \brief Gets a string value at given index on the stack of a Squirrel VM.
inline const char * getString(HSQUIRRELVM vm, SQInteger i)
{
    const char * str = nullptr;
    if (SQ_FAILED(sq_getstring(vm, 2, &str)))
        return nullptr;
    return str;
}

//------------------------------------------------------------------------------
/// \brief Gets a boolean value at given index on the stack of a Squirrel VM.
inline bool getBool(HSQUIRRELVM vm, SQInteger i)
{
	SQBool b;
	if (SQ_FAILED(sq_getbool(vm, i, &b)))
		return false;
	return b != 0;
}

//------------------------------------------------------------------------------
/// \brief Gets an integer value at given index on the stack of a Squirrel VM.
inline SQInteger getInt(HSQUIRRELVM vm, SQInteger i)
{
	SQInteger v;
	if (SQ_FAILED(sq_getinteger(vm, i, &v)))
		return 0;
	return v;
}

//------------------------------------------------------------------------------
/// \brief Gets a float value at given index on the stack of a Squirrel VM.
inline SQFloat getFloat(HSQUIRRELVM vm, SQInteger i)
{
	SQFloat v;
	if (SQ_FAILED(sq_getfloat(vm, i, &v)))
		return 0;
	return v;
}

//------------------------------------------------------------------------------
namespace priv
{
	/// \deprecated
    template<class T>
    void destroyClassInstance_checkRefCounted(std::true_type, T * p)
    {
        // If the class derives from RefCounted,
        // use its release() method instead of delete (which is protected)
        p->release();
    }

	/// \deprecated
    template<class T>
    void destroyClassInstance_checkRefCounted(std::false_type, T * p)
    {
        // If it's a regular class, just delete it
        delete p;
    }

	/// \deprecated
    template<class T>
    void destroyClassInstance(T * p)
    {
        priv::destroyClassInstance_checkRefCounted<T>(std::is_base_of<RefCounted, T>(), p);
    }
}

} // namespace squirrel

#endif // __HEADER_SN_SQ_BIND_TOOLS__


