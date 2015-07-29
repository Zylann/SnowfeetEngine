/*
Time_win32.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../Time.h"
#include <Windows.h>

namespace sn
{

LARGE_INTEGER getFrequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

// static
Time Time::getCurrent()
{
    // Force the following code to run on first core
    // (see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx)
    HANDLE currentThread = GetCurrentThread();
    DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

    // Get the frequency of the performance counter
    // (it is constant across the program lifetime)
    static LARGE_INTEGER frequency = getFrequency();

    // Get the current time
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    // Restore the thread affinity
    SetThreadAffinityMask(currentThread, previousMask);

    // Return the current time as microseconds
    return Time::microseconds(1000000 * time.QuadPart / frequency.QuadPart);
}

} // namespace sn

