#ifndef __HEADER_SQUIRREL_BIND_MACROS__
#define __HEADER_SQUIRREL_BIND_MACROS__

//
// This header defines helper macros to bind functions to Squirrel.
// Their name have been shortened for readability, also ONLY include them in a binding implementation file.
// Also, only use them exclusively in a private namespace where Squirrel wrappers are defined.
//

#include <core/util/macros.hpp>

// Put this on top of the namespace containing the wrappers
#define CURRENT_CLASS(_name) typedef _name __sn_CurrentClass;

#define BEGIN_METHOD(_name) \
	SQInteger _name(HSQUIRRELVM vm) { \
		auto * self = getNativeInstance<__sn_CurrentClass>(vm, 1); \
		if(self) {
		
		// Insert function code here

#define END_METHOD \
		} else { \
            return sq_throwerror(vm, "Attempt to call native method '" SN_FUNCNAME "' on destroyed instance"); \
		} \
	}

#endif // __HEADER_SQUIRREL_BIND_MACROS__


