/*
Thread.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Thread.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Thread::Thread(FuncType func):
    m_func(func)
{
}

//------------------------------------------------------------------------------
void Thread::run()
{
    if (m_func)
    {
        m_func();
    }
}

} // namespace sn

