/*
Thread_win32.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../Thread.hpp"
#include <core/types.hpp>
#include <core/util/assert.hpp>

#include <Windows.h>
#include <process.h>

namespace sn
{

//------------------------------------------------------------------------------
/// \cond INTERNAL
class ThreadImpl
{
public:

    ThreadImpl(Thread * owner) : 
        m_handle(0), 
        m_id(0) 
    {
        m_handle = reinterpret_cast<HANDLE>(_beginthreadex(
            NULL, // _Security
            0, // _StackSize
            &ThreadImpl::threadFunc, // Function to execute
            owner, // _ArgList: we send the Thread as userdata, see threadFunc below
            0, // _InitFlag
            &m_id // (out) win32 thread ID
        ));

        if (m_handle == NULL)
        {
            SN_ERROR("Failed to create thread");
        }
        else
        {
            SN_LOG("Creating thread " << m_id);
        }
    }

    ~ThreadImpl()
    {
        if (m_handle)
            CloseHandle(m_handle);
    }

    void terminate()
    {
        if (m_handle)
            TerminateThread(m_handle, 0);
    }

    void wait()
    {
        if (m_handle)
        {
            SN_ASSERT(m_id != GetCurrentThreadId(), "A thread cannot wait for itself!");
            WaitForSingleObject(m_handle, INFINITE);
        }
    }

    static unsigned int __stdcall ThreadImpl::threadFunc(void* userData)
    {
        // The Thread instance is stored in the user data
        Thread* owner = static_cast<Thread*>(userData);

        // Forward execution to the owner
        owner->run();

        // Optional, but it is cleaner
        _endthreadex(0);

        return 0;
    }

private:

    HANDLE m_handle;
    u32 m_id; // Win32 identifier
};
/// \endcond

//==============================================================================
// Thread
//==============================================================================

//------------------------------------------------------------------------------
Thread::~Thread()
{
    wait();
}

//------------------------------------------------------------------------------
void Thread::start()
{
    wait();
    m_impl = new ThreadImpl(this);
}

//------------------------------------------------------------------------------
void Thread::wait()
{
    if (m_impl)
    {
        m_impl->wait();
        delete m_impl;
        m_impl = NULL;
    }
}

//------------------------------------------------------------------------------
void Thread::terminate()
{
    if (m_impl)
    {
        m_impl->terminate();
        delete m_impl;
        m_impl = NULL;
    }
}

//------------------------------------------------------------------------------
// Static
void Thread::sleep(Time duration)
{
    ::Sleep(duration.asMilliseconds());
}

} // namespace sn

