/*
macros.hpp
Copyright (C) 2011-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MACROS__
#define __HEADER_SN_MACROS__

#include <core/config.hpp>

// Place this at the beginning of a code section that must be called once.
// It will raise a failing assertion if the code is called twice.
// Warning: it is designed as global code, not per-object instance.
#define SN_CALL_ONCE                                                           \
    static bool __callOnce = true;                                             \
    if(__callOnce)                                                             \
        __callOnce = false;                                                    \
    else                                                                       \
        assert(__callOnce)

#endif // __HEADER_SN_MACROS__


