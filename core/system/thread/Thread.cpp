/*
Thread.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Thread.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Thread::Thread(void(*func)()):
    r_func(func)
{
}

//------------------------------------------------------------------------------
void Thread::run()
{
    (*r_func)();
}

} // namespace sn

