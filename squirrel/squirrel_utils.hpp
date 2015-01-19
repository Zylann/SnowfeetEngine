#ifndef HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED
#define HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED

#include <stdarg.h>
#include <stdio.h>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>
#include <sqrat.h>

#ifdef _MSC_VER
	#pragma comment (lib, "squirrel.lib")
	#pragma comment (lib, "sqstdlib.lib")
#endif

namespace zn
{
namespace squirrel
{

void printfunc(HSQUIRRELVM vm, const SQChar *s,...);
void errorfunc(HSQUIRRELVM vm, const SQChar *s,...);

SQInteger registerGlobalFunction(HSQUIRRELVM vm, SQFUNCTION f, const char *fname);

void call(HSQUIRRELVM vm, const SQChar *fname);
void call(HSQUIRRELVM vm, const SQChar *fname, int n, float f, const SQChar *s);

inline void push(HSQUIRRELVM vm, SQInteger i)
{
	sq_pushinteger(vm, i);
}

inline void push(HSQUIRRELVM vm, SQFloat f)
{
	sq_pushfloat(vm, f);
}

inline void push(HSQUIRRELVM vm, SQBool b)
{
	sq_pushbool(vm, b);
}

inline void push(HSQUIRRELVM vm, const SQChar * s)
{
	sq_pushstring(vm, s, -1);
}

inline void push(HSQUIRRELVM vm, const std::string & s)
{
	sq_pushstring(vm, s.c_str(), s.size());
}

} // namespace squirrel
} // namespace zn

#endif // HEADER_ZN_SQUIRREL_UTILS_HPP_INCLUDED




