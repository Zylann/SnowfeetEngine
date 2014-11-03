/*
ArrayPainter2D.hpp
Copyright (c) 2013-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ARRAYPAINTER2D__
#define __HEADER_SN_ARRAYPAINTER2D__

#include <core/util/Array2D.hpp>

namespace sn
{

/// \brief Array2D editor providing convenience drawing-like functions
template <typename T>
class ArrayPainter2D
{
public:

    /// \brief Constructs the painter to edit the given 2D array
    ArrayPainter2D(Array2D<T> & arr) :
        a(arr)
    {}

    /// \brief Sets the (x,y) value
    void point(const T & value, s32 x, s32 y)
    {
        a.set(x, y);
    }

    /// \brief Fills the array with the given value
    void fill(const T & value)
    {
        a.fill(value);
    }

    /// \brief Fills a rectangle with the given value
    /// \brief value: value to set
    /// \brief x0: left corner of the rectangle
    /// \brief y0: upper corner of the rectangle
    /// \brief w: width of the rectangle
    /// \brief h: height of the rectangle
    void fillRect(const T & value, s32 x0, s32 y0, s32 w, s32 h)
    {
        for(s32 y = y0; y < y0+h; ++y)
        {
            for(s32 x = x0; x < x0+w; ++x)
            {
                a.set(x, y, value);
            }
        }
    }

    inline void fillRect(const T & value, Vector2i o, Vector2i size)
    {
        fillRect(value, o.x, o.y, size.x, size.y);
    }

    /// \brief Sets the inner-outline of a rectangle with the given value.
    /// \brief value: value to set
    /// \brief x0: left corner of the rectangle
    /// \brief y0: upper corner of the rectangle
    /// \brief w: width of the rectangle
    /// \brief h: height of the rectangle
    void strokeRect(const T & value, s32 x0, s32 y0, s32 w, s32 h)
    {
        for(s32 x = x0; x < x0+w; ++x)
        {
            a.set(x, y0, value);
            a.set(x, y0+h-1, value);
        }
        for(s32 y = y0+1; y < y0+h-1; ++y)
        {
            a.set(x0, y, value);
            a.set(x0+w-1, y, value);
        }
    }

    inline void strokeRect(const T & value, Vector2i o, Vector2i size)
    {
        strokeRect(value, o.x, o.y, size.x, size.y);
    }

    /// \brief Reference to the edited array
    Array2D<T> & a;

};

} // namespace sn

#endif // __HEADER_SN_ARRAYPAINTER2D__


