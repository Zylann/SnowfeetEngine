#include "VM.h"

#include <stdarg.h>
#include <stdio.h>

#include <sqstdaux.h>

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif

namespace squirrel
{

VM::VM(SQInteger initialStackSize):
    m_vm(sq_open(initialStackSize))
{
    // Set default print functions
    sq_setprintfunc(m_vm, defaultPrintFunc, defaultPrintFunc);

	// Registers default error handlers
	sqstd_seterrorhandlers(m_vm);
}

VM::~VM()
{
    sq_close(m_vm);
}

void VM::defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(s, vl);
    va_end(vl);
}


} // namespace squirrel

