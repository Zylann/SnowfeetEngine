/*
macros.h
Copyright (C) 2011-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MACROS__
#define __HEADER_SN_MACROS__

#include <core/config.h>

// Place this at the beginning of a block that must be called once.
// It will raise a failing assertion if the code is called twice.
// Warning: it is designed as global code, not per-object instance.
#define SN_CALL_ONCE                                                           \
    static bool __callOnce = true;                                             \
    if(__callOnce)                                                             \
        __callOnce = false;                                                    \
    else                                                                       \
        assert(__callOnce)

// Expands to the current function name
#define SN_FUNCNAME __FUNCTION__

// For-each macro working with STL containers
#define SN_FOREACH(iteratorName, containerName)\
    for(auto iteratorName = containerName.begin(); iteratorName != containerName.end(); ++iteratorName)


template <class T>
inline const char * macroImpl_typeString(const char * str)
{
    return str;
}

// Stringifies a type and produces a compilation error if the type is wrong
#define SN_TYPESTRING(_class) macroImpl_typeString<_class>(#_class)

#endif // __HEADER_SN_MACROS__


