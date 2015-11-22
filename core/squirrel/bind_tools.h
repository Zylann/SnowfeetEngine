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

/// \brief Explicit version of characters used in paramcheck masks.
/// Each character represents a Squirrel data type.
enum TypeChars
{
    T_NULL       = 'o',
    T_BOOL       = 'b',
    T_INT        = 'i',
    T_FLOAT      = 'f',
    T_STRING     = 's',
    T_TABLE      = 't',
    T_ARRAY      = 'a',
    T_USERDATA   = 'u',
    T_CLOSURE    = 'c',
    T_INSTANCE   = 'x',
    T_CLASS      = 'y',
    T_GENERATOR  = 'g',
    T_THREAD     = 'v',
    T_ANY        = '.'
};

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

//------------------------------------------------------------------------------
/// \brief Tests if a Squirrel type is numeric
inline bool isNumericType(SQObjectType t)
{
    return t == OT_FLOAT || t == OT_INTEGER;
}

//------------------------------------------------------------------------------
/// \brief Use this function to provide a typetag to sq_getinstanceup.
/// You have to define template specializations to make it work.
/// By default, it returns null and so ignores the tag check.
template <class T>
inline SQUserPointer getTypeTag()
{
    return nullptr;
}

//------------------------------------------------------------------------------
/// \brief Gets the associated userpointer of an instance at the given index on the stack of a Squirrel VM.
/// If getTypeTag has a specialization for T, it will be used to check the typetag.
/// If the typetag check fails, the function will return false.
template <typename T>
bool getNativeInstance(HSQUIRRELVM vm, SQInteger i, T *& out_ptr)
{
    SQUserPointer p = NULL;
    if (SQ_FAILED(sq_getinstanceup(vm, i, &p, getTypeTag<T>())))
        return false;
    out_ptr = static_cast<T*>(p);
    return true;
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

} // namespace squirrel

#endif // __HEADER_SN_SQ_BIND_TOOLS__


