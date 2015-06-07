/*
bind_tools.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SQ_BIND_TOOLS__
#define __HEADER_SN_SQ_BIND_TOOLS__

#include <squirrel.h>
#include <string>
//#include <core/app/ScriptableObject.hpp>

namespace sn
{
    /// \brief Prints the Squirrel stack in the console output
    void debugStackDump(HSQUIRRELVM v);

    void setGlobalFunction(HSQUIRRELVM vm, const char * name, SQFUNCTION cb_func);

    std::string getLastError(HSQUIRRELVM vm);

} // namespace sn

#endif // __HEADER_SN_SQ_BIND_TOOLS__


