/*
Time.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine.
*/

#ifndef __HEADER_SN_TIME__
#define __HEADER_SN_TIME__

#include <core/types.hpp>

namespace sn
{

class SN_API Time
{
public:

    Time() :
        m_microseconds(0)
    {}

    inline f32 asSeconds() const
    {
        return m_microseconds / 1000000.f;
    }

    inline s32 asMilliseconds() const
    {
        return static_cast<s32>(m_microseconds) / 1000;
    }

    inline s64 asMicroseconds() const
    {
        return m_microseconds;
    }

    static inline Time seconds(f32 s)
    {
        return Time(static_cast<s64>(s * 1000000.f));
    }

    static inline Time milliseconds(s32 ms)
    {
        return Time(ms * 1000);
    }

    static inline Time microseconds(s64 ms)
    {
        return Time(ms);
    }

    static Time getCurrent();

private:

    explicit Time(s64 microseconds):
        m_microseconds(microseconds)
    {}

    s64 m_microseconds;

};

inline bool operator ==(Time left, Time right)    { return left.asMicroseconds() == right.asMicroseconds(); }
inline bool operator !=(Time left, Time right)    { return left.asMicroseconds() != right.asMicroseconds(); }
inline bool operator <(Time left, Time right)     { return left.asMicroseconds() < right.asMicroseconds(); }
inline bool operator >(Time left, Time right)     { return left.asMicroseconds() > right.asMicroseconds(); }
inline bool operator <=(Time left, Time right)    { return left.asMicroseconds() <= right.asMicroseconds(); }
inline bool operator >=(Time left, Time right)    { return left.asMicroseconds() >= right.asMicroseconds(); }

inline Time operator -(Time right)                { return Time::microseconds(-right.asMicroseconds()); }
inline Time operator +(Time left, Time right)     { return Time::microseconds(left.asMicroseconds() + right.asMicroseconds()); }
inline Time operator -(Time left, Time right)     { return Time::microseconds(left.asMicroseconds() - right.asMicroseconds()); }
inline Time operator *(Time left, float right)    { return Time::seconds(left.asSeconds() * right); }
inline Time operator *(Time left, s64 right)      { return Time::microseconds(left.asMicroseconds() * right); }
inline Time operator *(float left, Time right)    { return right * left; }
inline Time operator *(s64 left, Time right)      { return right * left; }
inline Time operator /(Time left, float right)    { return Time::seconds(left.asSeconds() / right); }
inline Time operator /(Time left, s64 right)      { return Time::microseconds(left.asMicroseconds() / right); }

inline Time& operator +=(Time& left, Time right)  { return left = left + right; }
inline Time& operator -=(Time& left, Time right)  { return left = left - right; }
inline Time& operator *=(Time& left, float right) { return left = left * right; }
inline Time& operator *=(Time& left, s64 right)   { return left = left * right; }
inline Time& operator /=(Time& left, float right) { return left = left / right; }
inline Time& operator /=(Time& left, s64 right)   { return left = left / right; }

} // namespace sn

#endif // __HEADER_SN_TIME__


