#ifndef __HEADER_MOD_IMAGE_COMMON__
#define __HEADER_MOD_IMAGE_COMMON__

#include <core/export.h>

#ifdef SN_IMAGE_EXPORT
    #define SN_IMAGE_API SN_EXPORT_SPEC
#else
    #define SN_IMAGE_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_IMAGE_COMMON__
