#ifndef __HEADER_MOD_FREETYPE_COMMON__
#define __HEADER_MOD_FREETYPE_COMMON__

#include <core/export.h>

#ifdef SN_FREETYPE_EXPORT
    #define SN_FREETYPE_API SN_EXPORT_SPEC
#else
    #define SN_FREETYPE_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_FREETYPE_COMMON__

