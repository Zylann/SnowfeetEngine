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

void register_math(asIScriptEngine & e);

void register_Vector2i(asIScriptEngine & e);
void register_Color(asIScriptEngine & e);
void register_FloatRect(asIScriptEngine & e);
void register_IntRect(asIScriptEngine & e);


} // namespace sn

#endif // __HEADER_SN_AS_MATH__

