#ifndef __HEADER_MOD_TGUI_COMMON__
#define __HEADER_MOD_TGUI_COMMON__

#include <core/export.h>

#ifdef SN_TGUI_EXPORT
    #define SN_TGUI_API SN_EXPORT_SPEC
#else
    #define SN_TGUI_API SN_IMPORT_SPEC
#endif

#endif // __HEADER_MOD_TGUI_COMMON__
