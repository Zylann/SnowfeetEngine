/*
Vector2.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR3__
#define __HEADER_SN_VECTOR3__

#include <core/math/Vector.hpp>

namespace sn
{

/// Typedefs

typedef Vector<s32, 3> Vector3i;
typedef Vector<u32, 3> Vector3u;
typedef Vector<f32, 3> Vector3f;

template <typename T>
static inline Vector<T, 3> crossProduct(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
{
    return Vector<T, 3>(
        v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]
    );
}

} // namespace sn

#endif // __HEADER_SN_VECTOR3__



