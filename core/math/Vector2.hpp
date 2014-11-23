/*
Vector2.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR2__
#define __HEADER_SN_VECTOR2__

#include <core/math/Vector.hpp>

namespace sn
{

/// Typedefs

typedef Vector<s32,2> Vector2i;
typedef Vector<u32,2> Vector2u;
typedef Vector<f32,2> Vector2f;

} // namespace sn

/// Std Hash
namespace std
{
    template <>
    struct hash<sn::Vector2i>
    {
        // 32-bit integer (x,y) hash
        inline size_t operator()(const sn::Vector2i & v) const
        {
            return v[0] | (v[1] << 16);
        }
    };

} // namespace std

#endif // __HEADER_SN_VECTOR2__



