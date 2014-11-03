#include "../Mutex.hpp"
#include <Windows.h>

namespace sn
{

//------------------------------------------------------------------------------
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

