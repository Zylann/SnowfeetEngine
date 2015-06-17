#ifndef __HEADER_SN_SQUIRREL_BIND_MACROS__
#define __HEADER_SN_SQUIRREL_BIND_MACROS__

//
// This header defines helper macros to bind functions to Squirrel.
// Their name have been shortened for readability, also ONLY include them in a binding implementation file.
//

#include "../../util/macros.hpp"

#define BEGIN_METHOD(_name) \
	SQInteger _name(HSQUIRRELVM vm) { \
		auto * self = getNativeInstance<Entity>(vm, 1); \
		if(self) {
		
		// Insert function code here

#define END_METHOD \
		} else { \
			SN_ERROR("Attempt to call native method '" << SN_FUNCNAME << "' on destroyed instance"); \
			return 0; \
		} \
	}

#endif // __HEADER_SN_SQUIRREL_BIND_MACROS__


