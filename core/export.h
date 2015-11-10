#ifndef __HEADER_SN_EXPORTS__
#define __HEADER_SN_EXPORTS__

#ifndef SN_STATIC

    // SN_API is placed before each element we want to expose to the user of the dynamic library.
    // (Header-only definitions don't need that)

    #ifdef _WIN32

        #define SN_EXPORT_SPEC __declspec(dllexport)
        #define SN_IMPORT_SPEC __declspec(dllimport)

        // Windows compilers need specific (and different) keywords for export and import
        #ifdef SN_CORE_EXPORT
            #define SN_API SN_EXPORT_SPEC
        #else
            #define SN_API SN_IMPORT_SPEC
        #endif

        #define SN_MOD_EXPORT __declspec(dllexport)

        #ifdef _MSC_VER
            // Visual C++: disable warning about STL classes needing export
            // Note: this is because STL versions differ across compilers.
            // However, it should be fine if we compile the engine and its modules with the same compiler.
            #pragma warning(disable : 4251)
        #endif

    #else // Linux, FreeBSD, Mac OS X
        #if __GNUC__ >= 4
            #define SN_EXPORT_SPEC __attribute__ ((__visibility__ ("default")))
            #define SN_IMPORT_SPEC SN_EXPORT_SPEC

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define SN_API SN_EXPORT_SPEC
        #else
            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define SN_EXPORT_SPEC
            #define SN_IMPORT_SPEC
            #define SN_API
        #endif
    #endif

#else
    // Static build doesn't need import/export macros
    #define SN_API
#endif

#endif // __HEADER_SN_EXPORTS__


