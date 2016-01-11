#ifndef __HEADER_SQUIRREL_BIND_MACROS__
#define __HEADER_SQUIRREL_BIND_MACROS__

//
// This header defines helper macros to bind functions to Squirrel.
// Their name have been shortened for readability, so ONLY include them in a binding implementation file.
// Also, only use them exclusively in a private namespace where Squirrel wrappers are defined.
//

#include <core/util/macros.h>

// Put this on top of the namespace containing the wrappers
#define CURRENT_CLASS(_name) typedef _name squirrelBinding_CurrentClass;

// Put this on top of each method. It will create the 'self' variable containing the C++ object,
// and perform all required checks on it.
#define GET_SELF() \
    squirrelBinding_CurrentClass * self = nullptr; \
    do {\
        SQUserPointer ptr = nullptr; \
        if (SQ_FAILED(sq_getinstanceup(vm, 1, &ptr, squirrel::getTypeTag<squirrelBinding_CurrentClass>()))) {\
            return sq_throwerror(vm, "sq_getinstanceup failed in native method '" SN_FUNCNAME "'");\
        }\
        if (ptr == nullptr)\
            return sq_throwerror(vm, "Attempt to call native method '" SN_FUNCNAME "' on destroyed instance"); \
        self = static_cast<squirrelBinding_CurrentClass*>(ptr);\
    } while (0)

#endif // __HEADER_SQUIRREL_BIND_MACROS__


