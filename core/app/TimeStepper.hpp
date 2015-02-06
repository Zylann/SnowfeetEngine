/*
TimeStepper.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_TIME_STEPPER__
#define __HEADER_SN_TIME_STEPPER__

#include <core/system/time/Clock.hpp>
#include <vector>

namespace sn
{

/// \brief This class encapsulates time management for mainLoop-based applications,
/// and provides an algorithm for smoothing time delta variations, resulting in
/// a smoother experience (less little jitterings).
class SN_API TimeStepper
{
public:

    /// \brief Creates a TimeStepper with default parameters.
    TimeStepper() :
        m_fps(0),
        m_recordedFPS(60),
        m_minDelta(sn::Time::seconds(1.f / 120.f)),
        m_maxDelta(sn::Time::seconds(1.f / 60.f))
    {
        m_rawDelta = m_maxDelta;
    }

    /// \brief Gets eleapsed time from the TimeStepper's creation.
    inline sn::Time getTime() const
    {
        return m_time.getElapsedTime();
    }

    /// \brief Gets the measured delta time between onBeginFrame() and onEndFrame().
    inline sn::Time getRawDelta() const
    {
        return m_rawDelta;
    }

    /// \brief Sets the expected range for the time delta.
    /// \param minDelta: minimum time interval
    /// \param minDelta: maximum time interval
    inline void setDeltaRange(sn::Time minDelta, sn::Time maxDelta)
    {
        m_minDelta = minDelta;
        m_maxDelta = maxDelta;
    }

    /// \brief Gets the minimal expected time delta
    inline sn::Time getMinDelta() const
    {
        return m_minDelta;
    }

    /// \brief Gets the maximal expected time delta
    inline sn::Time getMaxDelta() const
    {
        return m_maxDelta;
    }

    /// \brief Gets the number of frames measured in 1 second.
    /// This value is not instant, so it can be zero when you start using the TimeStepper.
    inline u32 getRecordedFPS() const
    {
        return m_recordedFPS;
    }

    /// \brief Call this function when a frame begins (before events, updates or rendering).
    void onBeginFrame();
    /// \brief Call this function when a frame ends (after event, updates or rendering).
    void onEndFrame();

    /// \brief Use this function to call a regular-time-delta step function (as your game logic update).
    /// depending on the current delta time, the provided callback will be called once or several times
    /// in order to get a fixed-time step logic.
    /// \param stepFunc: update callback to call
    /// \return number of times the given callback has been called
    //u32 callSteps(void(*stepFunc)(sn::Time, void*), void* userdata);
    //u32 callSteps(std::function<void(sn::Time)> stepFunc);

    std::vector<sn::Time> getCallDeltas();

private:

    u32 m_fps;
    u32 m_recordedFPS;
    sn::Time m_lastFPS;

    sn::Time m_minDelta;
    sn::Time m_maxDelta;
    sn::Time m_timeBefore;
    sn::Time m_rawDelta;
    sn::Time m_storedDelta;
    sn::Clock m_time;

};

} // namespace sn

#endif // __HEADER_SN_TIME_STEPPER__

