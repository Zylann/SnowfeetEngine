/*
SampledCurve.hpp
Copyright (c) Marc GILLERON (2013-2015)
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SAMPLEDCURVE__
#define __HEADER_SN_SAMPLEDCURVE__

#include <cassert>
#include <functional>
#include <core/types.hpp>

namespace sn
{

/// \brief Represents a precalculated float/double function.
template <typename Float_T, u32 _Size>
class SampledCurve
{
public:

    /// \brief constructs a sampled curve from a function (can be used with sin, cos...)
    SampledCurve(std::function<Float_T(Float_T)> f, Float_T minX, Float_T maxX)
    {
        sample(f, minX, maxX);
    }

    /// \brief constructs the curve from a function (can be used with sin, cos...)
    void sample(std::function<Float_T(Float_T)> f, Float_T minX, Float_T maxX)
    {
        assert(minX < maxX);

        m_minX = minX;
        m_maxX = maxX;

        m_inverseWidth = 1.0 / (m_maxX - m_minX);

        for(u32 i = 0; i < _Size; ++i)
        {
            Float_T t = static_cast<Float_T>(i) / static_cast<Float_T>(_Size);
            m_lookup[i] = f(m_minX + t * (m_maxX - m_minX));
        }
    }

    /// \brief gets the value of the curve at the given coordinate X.
    /// values are clamped to the definition interval.
    Float_T getClamp(Float_T x)
    {
        u32 i = getIndex(x);

        if(i >= _Size)
            i = _Size-1;
        else if(i < 0)
            i = 0;

        return m_lookup[i];
    }

    /// \brief gets the value of the curve at the given coordinate X.
    /// values are repeated on the definition interval (useful for trigonometric functions).
    Float_T getRepeat(Float_T x)
    {
        u32 i = getIndex(x) % _Size;
        return m_lookup[i];
    }

    // TODO getClamp linear and getRepeat linear

private:

    inline u32 getIndex(Float_T x)
    {
        return static_cast<u32>(static_cast<f32>(_Size) * (x - m_minX) * m_inverseWidth);
    }

    Float_T m_lookup[_Size];
    Float_T m_minX;
    Float_T m_maxX;
    Float_T m_inverseWidth;

};

} // namespace sn

#endif // __HEADER_SN_SAMPLEDCURVE__


