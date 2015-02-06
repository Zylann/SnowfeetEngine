/*
Thread.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_THREAD__
#define __HEADER_SN_THREAD__

#include <core/util/NonCopyable.hpp>
#include <core/system/time/Time.hpp>

namespace sn
{

class ThreadImpl;

class SN_API Thread : NonCopyable
{
public:
    Thread(void(*func)());
    ~Thread();

    void start();
    void wait();
    void terminate();

    static void sleep(Time duration);

private:
    friend class ThreadImpl;

    void run();

    void (*r_func)();
    ThreadImpl * m_impl;
};

} // namespace sn

#endif // __HEADER_SN_THREAD__


