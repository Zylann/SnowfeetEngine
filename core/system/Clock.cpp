/*
Clock.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Clock.h"

namespace sn
{

//------------------------------------------------------------------------------
Clock::Clock():
    m_startTime(Time::getCurrent())
{
}

//------------------------------------------------------------------------------
Time Clock::getElapsedTime() const
{
    return Time::getCurrent() - m_startTime;
}

//------------------------------------------------------------------------------
Time Clock::restart()
{
    Time now = Time::getCurrent();
    Time elapsed = now - m_startTime;
    m_startTime = now;

    return elapsed;
}

} // namespace sn

