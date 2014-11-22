/*
Rect.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_RECT__
#define __HEADER_SN_RECT__

#include <core/types.hpp>

namespace sn
{

/// \brief a rectangle defined by an origin and a size.
template <typename T>
struct Rect
{
    T x;
    T y;
    T width;
    T height;

    //-----------------------------
    static inline Rect fromPositionSize(const T & x_, const T & y_, const T & w_, const T & h_)
    {
        return Rect(x_, y_, w_, h_);
    }

    //-----------------------------
    static Rect fromMinMax(T minX, T minY, T maxX, T maxY)
    {
        if (minX > maxX)
        {
            T temp = maxX;
            maxX = minX;
            minX = temp;
        }
        if (minY > maxY)
        {
            T temp = maxY;
            maxY = minY;
            minY = temp;
        }
        return Rect(minX, minY, maxX - minX, maxY - minY);
    }

    //-----------------------------
    Rect() :
        x(0),
        y(0),
        width(0),
        height(0)
    {}

    //-----------------------------
    Rect(const T & x_, const T & y_, const T & width_, const T & height_) :
        x(x_),
        y(y_),
        width(width_),
        height(height_)
    {}

    //-----------------------------
    void set(const T & x_, const T & y_, const T & width_, const T & height_)
    {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }

    //-----------------------------
    bool contains(const T & px, const T & py) const
    {
        return px >= x && py >= py && px < x + width && py < y + height;
    }

    //-----------------------------
    inline T minX() const { return x; }
    inline T minY() const { return y; }
    inline T maxX() const { return x + width; }
    inline T maxY() const { return x + height; }

};

template <typename T>
bool operator!=(const Rect<T> & a, const Rect<T> & b)
{
    return a.x != b.x 
        && a.y != b.y 
        && a.width != b.width 
        && a.height != b.height;
}

typedef Rect<s32> IntRect;
typedef Rect<f32> FloatRect;

} // namespace sn

#endif // __HEADER_SN_RECT__



