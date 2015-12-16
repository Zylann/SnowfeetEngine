/*
Range.h
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_RANGE__
#define __HEADER_SN_RANGE__

#include <iostream>
#include <core/types.h>

namespace sn
{

template <typename T>
class Range
{
public:

    Range() : m_min(0), m_max(0) {}
    Range(T min, T max) { set(min, max); }

    void set(T min, T max)
    {
        if(min > max)
        {
            m_min = max;
            m_max = min;
        }
        else
        {
            m_min = min;
            m_max = max;
        }
    }

    void setMin(T min)
    {
        if (min >= m_max)
            m_max = min;
        m_min = min;
    }

    void setMax(T max)
    {
        if (max <= m_min)
            m_min = max;
        m_max = max;
    }

    inline T width() const { return m_max - m_min; }

    inline f32 inverseLerp(T v)
    {
        return static_cast<f32>(v - m_min) / static_cast<f32>(width());
    }

    inline f32 lerp(f32 t)
    {
        return m_min + t * width();
    }

    inline T min() const { return m_min; }
    inline T max() const { return m_max; }

    inline bool contains(T x) const { return x >= m_min && x <= m_max; }

    inline void print(std::ostream & os) const
    {
        os << "[" << m_min << ", " << m_max << "]";
    }

private:

    T m_min;
    T m_max;

};

} // namespace sn

#endif // __HEADER_SN_RANGE__



