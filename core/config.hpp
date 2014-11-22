//
// config.hpp
// Copyright (C) 2010-2014 Marc GILLERON
// This file is part of the SnowfeetEngine project.
//

#ifndef __HEADER_SN_CONFIG__
#define __HEADER_SN_CONFIG__

#define SN_VERSION 141101
#define SN_VERSION_STRING "14.11.1"

//------------------------------------------------------------------------------
// Switches
// Note: these #defines can either be active here or in your build commands.

// Enables debug code (additionnal checks and messages, may decrease performance)
//#define SN_BUILD_DEBUG

// Enables static compilation config.
//#define SN_STATIC

// [Deprecated][Obsolete]
// Hint to ease SFML integration
//#define SN_SFML

//------------------------------------------------------------------------------
// Exports

#ifndef SN_STATIC

    // SN_API is placed before each element we want to expose to the user of the dynamic library.
    // (Header-only definitions don't need that)

    #ifdef _WIN32

        // Windows compilers need specific (and different) keywords for export and import
        #define SN_API __declspec(dllexport)

        #ifdef _MSC_VER
            // Visual C++: disable warning about STL classes needing export
            // Note: this is because STL versions differ across compilers.
            // However, it should be fine if we compile the engine and its modules with the same compiler.
            #pragma warning(disable : 4251)
        #endif

    #else // Linux, FreeBSD, Mac OS X
        #if __GNUC__ >= 4
            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define SN_API __attribute__ ((__visibility__ ("default")))
        #else
            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define SN_API
        #endif
    #endif
#else
    // Static build doesn't need import/export macros
    #define SN_API
#endif

//------------------------------------------------------------------------------
// C++11 compatibility
#ifdef _MSC_VER
    #define SN_NOEXCEPT _NOEXCEPT
#else
    #define SN_NOEXCEPT noexcept
#endif

#endif // __HEADER_SN_CONFIG__





