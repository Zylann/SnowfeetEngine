/*
as_math.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_AS_MATH__
#define __HEADER_SN_AS_MATH__

#include <angelscript.h>

namespace sn
{

// Note: for convenience, functions below are implemented in separate files.
// The implementation files start with as_*.

void register_math(asIScriptEngine * engine);
void register_Vector2i(asIScriptEngine * engine);

} // namespace sn

#endif // __HEADER_SN_AS_MATH__

