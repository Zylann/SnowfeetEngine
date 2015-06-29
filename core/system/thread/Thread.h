/*
Thread.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_THREAD__
#define __HEADER_SN_THREAD__

#include <core/util/NonCopyable.h>
#include <core/system/time/Time.h>
#include <functional>

namespace sn
{

class ThreadImpl;

class SN_API Thread : NonCopyable
{
public:
    typedef std::function<void()> FuncType;

    Thread(FuncType func);
    ~Thread();

    void start();
    void wait();
    void terminate();

    static void sleep(Time duration);

private:
    friend class ThreadImpl;

    void run();

    FuncType m_func;
    ThreadImpl * m_impl;
};

} // namespace sn

#endif // __HEADER_SN_THREAD__


