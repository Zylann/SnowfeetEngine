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

// Just an idea...
//#define DECLARE_SQUIRREL_CLASS_NS(_namespace, _class)\
//    namespace _namespace {\
//        extern squirrel::ClassBinding _class##_squirrelBinding;\
//    }\
//    namespace squirrel {\
//        template <>\
//        inline ClassBinding & getClassBinding<_namespace::##_class>() {\
//            return _namespace::_class##_squirrelBinding;\
//        }\
//    }
//
//#define IMPL_SQUIRREL_CLASS_NS(_namespace, _class)\
//    namespace _namespace {\
//        squirrel::ClassBinding _class##_squirrelBinding;\
//        typedef _class squirrelBinding_CurrentClass;\
//    }

namespace squirrel
{

//------------------------------------------------------------------------------
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
//class ClassBinding
//{
//};

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
//template <class T>
//inline ClassBinding & getClassBinding()
//{
//    static_assert(false, "Specialization not found");
//    return nullptr;
//}

//------------------------------------------------------------------------------
/// \brief Use this function to provide a unique typetag to sq_getinstanceup.
/// You have to define template specializations to make it work.
/// Otherwise, it will fail because a value of 0 would disable typetag checking
/// and make sq_getinstanceup calls unsafe (it's a cast from void*, you know)
///
/// \note A common implementation is to simply return the address of the function or
/// object providing the binding, which is guaranteed to be unique during process execution.
///
template <class T>
inline SQUserPointer getTypeTag()
{
    //return getClassBinding<T>();
    static_assert(false, "getTypeTag<T> Specialization not found.");
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
inline const SQChar * getString(HSQUIRRELVM vm, SQInteger i, const SQChar * defval = nullptr)
{
    const SQChar * str = nullptr;
    if (SQ_FAILED(sq_getstring(vm, i, &str)))
        return defval;
    return str;
}

//------------------------------------------------------------------------------
/// \brief Gets a boolean value at given index on the stack of a Squirrel VM.
inline bool getBool(HSQUIRRELVM vm, SQInteger i, bool defval = false)
{
	SQBool b;
	if (SQ_FAILED(sq_getbool(vm, i, &b)))
		return defval;
	return b != 0;
}

//------------------------------------------------------------------------------
/// \brief Gets an integer value at given index on the stack of a Squirrel VM.
inline SQInteger getInt(HSQUIRRELVM vm, SQInteger i, SQInteger defval = 0)
{
	SQInteger v;
	if (SQ_FAILED(sq_getinteger(vm, i, &v)))
		return defval;
	return v;
}

//------------------------------------------------------------------------------
/// \brief Gets a float value at given index on the stack of a Squirrel VM.
inline SQFloat getFloat(HSQUIRRELVM vm, SQInteger i, SQFloat defval = 0)
{
	SQFloat v;
	if (SQ_FAILED(sq_getfloat(vm, i, &v)))
		return defval;
	return v;
}

//------------------------------------------------------------------------------
inline SQRESULT getValue(HSQUIRRELVM vm, SQInteger i, const SQChar *& out_value)
{
    return sq_getstring(vm, i, &out_value);
}

//------------------------------------------------------------------------------
inline SQRESULT getValue(HSQUIRRELVM vm, SQInteger i, std::string & out_value)
{
    const SQChar * str;
    SQRESULT res = sq_getstring(vm, i, &str);
    if (SQ_SUCCEEDED(res))
        out_value = str;
    return res;
}

//------------------------------------------------------------------------------
inline SQRESULT getValue(HSQUIRRELVM vm, SQInteger i, SQBool & out_value)
{
    return sq_getbool(vm, i, &out_value);
}

//------------------------------------------------------------------------------
inline SQRESULT getValue(HSQUIRRELVM vm, SQInteger i, SQInteger & out_value)
{
    return sq_getinteger(vm, i, &out_value);
}

//------------------------------------------------------------------------------
inline SQRESULT getValue(HSQUIRRELVM vm, SQInteger i, SQFloat & out_value)
{
    return sq_getfloat(vm, i, &out_value);
}

//------------------------------------------------------------------------------
template <typename T>
T tryGetValue(HSQUIRRELVM vm, SQInteger i, T defval = T())
{
    T value;
    if (SQ_FAILED(getValue(vm, i, value)))
        return defval;
    return value;
}

} // namespace squirrel

#endif // __HEADER_SN_SQ_BIND_TOOLS__


