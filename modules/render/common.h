#ifndef __HEADER_MOD_RENDER_COMMON__
#define __HEADER_MOD_RENDER_COMMON__

#include <core/export.h>

#ifdef SN_RENDER_EXPORT
    #define SN_RENDER_API SN_EXPORT_SPEC
#else
    #define SN_RENDER_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_RENDER_COMMON__
