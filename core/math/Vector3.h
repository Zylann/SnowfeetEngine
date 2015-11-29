/*
Vector2.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR3__
#define __HEADER_SN_VECTOR3__

#include <core/math/Vector.h>

namespace sn
{

/// Typedefs

typedef Vector<s32, 3> Vector3i;
typedef Vector<u32, 3> Vector3u;
typedef Vector<f32, 3> Vector3f;

template <typename T>
inline Vector<T, 3> crossProduct(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
{
    return Vector<T, 3>(
        v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]
    );
}

/// \brief Gets spherical coordinates angles (euler angles) of the given vector in degrees.
/// Assumes the vertical vector to be (0,1,0).
/// Note: the roll angle will always be zero.
template <typename T>
inline Vector<f32, 3> getSphericalAngles(const Vector<T, 3> & v)
{
    Vector<f32, 3> angles;
    f32 lengthSQ = v.getLengthSQ();
    if (lengthSQ)
    {
        if (v.x() != 0)
            angles.y() = (T)( atan2((f64)v.z(), (f64)v.x()) * math::RAD2DEG64 );
        else if (v.z() < 0)
            angles.y() = 180;
        angles.x() = (T)(acos(v.y() * sqrt(1.0/(lengthSQ))) * math::RAD2DEG64);
    }
    return angles;
}

inline size_t getHash(const Vector3i & v)
{
    return (((size_t)v.x()) * 73856093)
		^ (((size_t)v.y()) * 19349663)
		^ (((size_t)v.z()) * 83492791);
}

} // namespace sn

/// Std Hash
namespace std
{
    template <>
    struct hash<sn::Vector3i>
    {
        // 32-bit integer (x,y) hash
        inline size_t operator()(const sn::Vector3i & v) const
        {
            return sn::getHash(v);
        }
    };

} // namespace std

#endif // __HEADER_SN_VECTOR3__



