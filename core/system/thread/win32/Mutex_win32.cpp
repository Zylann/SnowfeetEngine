/*
Mutex_win32.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../Mutex.hpp"
#include <Windows.h>

namespace sn
{

//------------------------------------------------------------------------------
/// \cond INTERNAL
class MutexImpl
{
public:

    MutexImpl::MutexImpl()
    {
        InitializeCriticalSection(&m_criticalSection);
    }

    MutexImpl::~MutexImpl()
    {
        DeleteCriticalSection(&m_criticalSection);
    }

    void MutexImpl::lock()
    {
        EnterCriticalSection(&m_criticalSection);
    }

    void MutexImpl::unlock()
    {
        LeaveCriticalSection(&m_criticalSection);
    }

private:

    CRITICAL_SECTION m_criticalSection;
};
/// \endcond

//==============================================================================
// Mutex
//==============================================================================

//------------------------------------------------------------------------------
Mutex::Mutex()
{
    m_impl = new MutexImpl();
}

//------------------------------------------------------------------------------
Mutex::~Mutex()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
void Mutex::lock()
{
    m_impl->lock();
}

//------------------------------------------------------------------------------
void Mutex::unlock()
{
    m_impl->unlock();
}

} // namespace sn

