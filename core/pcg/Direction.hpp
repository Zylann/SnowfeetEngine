/*
Direction.hpp
Copyright (C) 2012-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef __HEADER_SN_DIRECTION__
#define __HEADER_SN_DIRECTION__

#include <core/types.hpp>

//#define SN_Y_UP

namespace sn
{

// Y-down 2D 4-direction helper
namespace Direction
{

    // Contiguous direction identifiers
    enum
    {
        RIGHT = 0,
        LEFT  = 1,
        UP    = 2,
        DOWN  = 3,
        NONE  = 4
    };

    // Bitmasks
    enum
    {
        LEFT_BIT   = 1 << RIGHT,
        RIGHT_BIT  = 1 << LEFT,
        UP_BIT     = 1 << UP,
        DOWN_BIT   = 1 << DOWN,
        NONE_BITS  = 0,
        ANY_BITS   = LEFT_BIT | RIGHT_BIT | UP_BIT | DOWN_BIT
    };

    u8 opposite(u8 dir)
    {
        switch(dir)
        {
        case RIGHT: return LEFT;
        case LEFT: return RIGHT;
        case UP: return DOWN;
        case DOWN: return UP;
        default: return NONE;
        }
    }

    u8 left(u8 dir)
    {
        switch(dir)
        {
        case RIGHT: return UP;
        case LEFT: return DOWN;
        case UP: return LEFT;
        case DOWN: return RIGHT;
        default: return NONE;
        }
    }

    u8 right(u8 dir)
    {
        switch(dir)
        {
        case RIGHT: return DOWN;
        case LEFT: return UP;
        case UP: return RIGHT;
        case DOWN: return LEFT;
        default: return NONE;
        }
    }

    template <typename T>
    sn::Vector<T, 2> toVector(u8 dir)
    {
        switch(dir)
        {
        case RIGHT: return sn::Vector<T,2>(1, 0);
        case LEFT: return sn::Vector<T,2>(-1, 0);
#ifdef SN_Y_UP
        case UP: return sn::Vector<T,2>(0, 1);
        case DOWN: return sn::Vector<T,2>(0, -1);
#else
        case UP: return sn::Vector<T,2>(0, -1);
        case DOWN: return sn::Vector<T,2>(0, 1);
#endif
        default: return sn::Vector<T,2>(0,0);
        }
    }

    template <typename T>
    u8 toDirectionOrtho(const sn::Vector<T,2> & v)
    {
        if(v.x() > 0)
            return RIGHT;
        if(v.x() < 0)
            return LEFT;
#ifdef SN_Y_UP
        if(v.y() > 0)
            return UP;
        if(v.y() < 0)
            return DOWN;
#else
        if(v.y() > 0)
            return DOWN;
        if(v.y() < 0)
            return UP;
#endif
        return NONE;
    }

    template <typename T>
    u8 toHorizontalDirection(T x)
    {
        if(x > 0)
            return Direction::RIGHT;
        else if(x < 0)
            return Direction::LEFT;
        else
            return Direction::NONE;
    }

    template <typename T>
    u8 toVerticalDirection(T y)
    {
#ifdef SN_Y_UP
        if(y > 0)
            return Direction::UP;
        else if(y < 0)
            return Direction::DOWN;
#else
        if(y > 0)
            return Direction::DOWN;
        else if(y < 0)
            return Direction::UP;
#endif
        else
            return Direction::NONE;
    }

    template <typename T>
    u8 toDirectionApprox(const sn::Vector<T,2> & v)
    {
        if(v.x() > 0)
        {
            if(v.y() > 0)
            {
                if(v.x() > v.y())
                    return RIGHT;
                else
#ifdef SN_Y_UP
                    return UP;
#else
                    return DOWN;
#endif
            }
            else if(v.y() < 0)
            {
                if(v.x() > -v.y())
                    return RIGHT;
                else
#ifdef SN_Y_UP
                    return DOWN;
#else
                    return UP;
#endif
            }
        }
        else if(v.x() < 0)
        {
            if(v.y()() > 0)
            {
                if(-v.x() > v.y())
                    return LEFT;
                else
#ifdef SN_Y_UP
                    return UP;
#else
                    return DOWN;
#endif
            }
            else if(v.y() < 0)
            {
                if(-v.x() > -v.y())
                    return LEFT;
                else
#ifdef SN_Y_UP
                    return DOWN;
#else
                    return UP;
#endif
            }
        }

        return NONE;
    }

    inline std::string toString(u8 dir)
    {
        switch(dir)
        {
        case RIGHT: return "right";
        case LEFT: return "left";
        case UP: return "up";
        case DOWN: return "down";
        default: return "none";
        }
    }

};

} // namespace sn

#endif // __HEADER_SN_DIRECTION__


