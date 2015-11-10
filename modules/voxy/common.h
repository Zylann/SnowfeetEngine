#ifndef __HEADER_MOD_VOXY_COMMON__
#define __HEADER_MOD_VOXY_COMMON__

#include <core/export.h>

#ifdef SN_VOXY_EXPORT
    #define SN_VOXY_API SN_EXPORT_SPEC
#else
    #define SN_VOXY_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_VOXY_COMMON__
