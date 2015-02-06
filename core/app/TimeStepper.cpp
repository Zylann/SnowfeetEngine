/*
TimeStepper.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "TimeStepper.hpp"

namespace sn
{

//------------------------------------------------------------------------------
void TimeStepper::onBeginFrame()
{
    Time t = getTime();

    m_timeBefore = t;

    // Record FPS
    if (t - m_lastFPS > Time::milliseconds(1000))
    {
        m_lastFPS = t;
        m_recordedFPS = m_fps;
        m_fps = 0;
    }
    ++m_fps;
}

//------------------------------------------------------------------------------
void TimeStepper::onEndFrame()
{
    m_rawDelta = getTime() - m_timeBefore;
    if (m_rawDelta > m_maxDelta)
    {
        m_rawDelta = m_maxDelta;
    }
}

//------------------------------------------------------------------------------
std::vector<sn::Time> TimeStepper::getCallDeltas()
{
    std::vector<Time> deltas;

    // Get current delta
    Time delta = m_rawDelta;
    if (m_recordedFPS != 0)
    {
        delta = Time::milliseconds(1000 / m_recordedFPS);
    }

    u32 updatesCount = 0;

    // Accumulate delta time
    m_storedDelta += delta;

    // If the accumulated delta time is enough to trigger an update step
    if (m_storedDelta >= m_minDelta)
    {
        // Call update steps one or more times to match the elapsed time
        s32 cycles = m_storedDelta.asMilliseconds() / m_maxDelta.asMilliseconds();
        for (s32 i = 0; i < cycles; ++i)
        {
            deltas.push_back(m_maxDelta);
            ++updatesCount;
        }

        // Update remainder time
        Time remainder = Time::milliseconds(m_storedDelta.asMilliseconds() % m_maxDelta.asMilliseconds());
        if (remainder > m_minDelta)
        {
            deltas.push_back(remainder);
            m_storedDelta = Time::milliseconds(0);
            ++updatesCount;
        }
        else
        {
            m_storedDelta = remainder;
        }
    }

    return deltas;
}

} // namespace sn

