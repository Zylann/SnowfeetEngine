#ifndef __HEADER_MOD_OCULUS_COMMON__
#define __HEADER_MOD_OCULUS_COMMON__

#include <core/export.h>

#ifdef SN_OCULUS_EXPORT
    #define SN_OCULUS_API SN_EXPORT_SPEC
#else
    #define SN_OCULUS_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_OCULUS_COMMON__
