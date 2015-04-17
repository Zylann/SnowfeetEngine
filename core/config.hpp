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
// Compiler

#ifdef _WIN32
    #ifdef _MSC_VER
        // Visual C++: disable warning about very long type names.
        // It happens frequently when template containers are nested.
        #pragma warning(disable : 4503)
    #endif
#endif

//------------------------------------------------------------------------------
#include <core/export.hpp>

//------------------------------------------------------------------------------
// C++11 compatibility
#ifdef _MSC_VER
    #define SN_NOEXCEPT _NOEXCEPT
#else
    #define SN_NOEXCEPT noexcept
#endif

#endif // __HEADER_SN_CONFIG__





