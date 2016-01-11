/*
Mutex.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MUTEX__
#define __HEADER_SN_MUTEX__

#include <core/util/NonCopyable.h>

namespace sn
{

class MutexImpl;

class SN_API Mutex : NonCopyable
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

private:
    MutexImpl * m_impl;
};

} // namespace sn

#endif // __HEADER_SN_MUTEX__

