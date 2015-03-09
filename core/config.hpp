//
// config.hpp
// Copyright (C) 2010-2015 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#ifndef __HEADER_SN_CONFIG__
#define __HEADER_SN_CONFIG__

#define SN_VERSION 150127
#define SN_VERSION_STRING "15.01.27"

//------------------------------------------------------------------------------
// Switches
// Note: these #defines can either be active here or in your build commands.

// Enables debug code (additionnal checks and messages, may decrease performance)
//#define SN_BUILD_DEBUG

// Enables static compilation config.
//#define SN_STATIC

//------------------------------------------------------------------------------
#include <core/export.hpp>

//------------------------------------------------------------------------------
// C++11 compatibility
#ifdef _MSC_VER
    #define SN_NOEXCEPT _NOEXCEPT
#else
    #define SN_NOEXCEPT noexcept
#endif

//------------------------------------------------------------------------------
#define SN_USE_MEMORY_MANAGER
#ifdef SN_USE_MEMORY_MANAGER
#include <core/system/memory/operators.hpp>
#endif

//#ifdef _MSC_VER
//    #define _CRTDBG_MAP_ALLOC
//
//    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//    #define new DEBUG_NEW
//
//    #include <stdlib.h>
//    #include <crtdbg.h>
//#endif

#endif // __HEADER_SN_CONFIG__





