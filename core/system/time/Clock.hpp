﻿/*
Clock.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine.
*/

#ifndef __HEADER_SN_CLOCK__
#define __HEADER_SN_CLOCK__

#include <core/system/time/Time.hpp>

namespace sn
{

class SN_API Clock
{
public:

    Clock();

    Time getElapsedTime() const;

    Time restart();

private:

    /// \brief time of last reset
    Time m_startTime;

};

} // namespace sn

#endif // __HEADER_SN_CLOCK__


