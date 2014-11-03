/*
math.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine.
*/

#ifndef __HEADER_SN_MATH__
#define __HEADER_SN_MATH__

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <core/types.hpp>

namespace sn {
namespace math {

const f32 ROUNDING_ERROR_F32 = 0.000001f; // Float epsilon
const f64 ROUNDING_ERROR_F64 = 0.00000001; // Double epsilon

/// Constant for PI.
const f32 PI = 3.14159265359f;

#ifdef PI64 // make sure we don't collide with a define
#undef PI64
#endif

/// Constant for 64bit PI.
const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;

/// 32bit Constant for converting from degrees to radians
const f32 DEG2RAD = PI / 180.0f;

/// 32bit constant for converting from radians to degrees
const f32 RAD2DEG = 180.0f / PI;

/// 64bit constant for converting from degrees to radians
const f64 DEG2RAD64 = PI64 / 180.0;

/// 64bit constant for converting from radians to degrees
const f64 RAD2DEG64 = 180.0 / PI64;

/// Utility function to convert a radian value to degrees
inline f32 radToDeg(f32 radians)
{
    return RAD2DEG * radians;
}

/// Utility function to convert a degrees value to radians
inline f32 degToRad(f32 degrees)
{
    return DEG2RAD * degrees;
}

/// Utility function to convert a 64bit radian value to degrees
inline f64 radToDeg(f64 radians)
{
    return RAD2DEG64 * radians;
}

/// Utility function to convert a 64bit degrees value to radians
inline f64 degToRad(f64 degrees)
{
    return DEG2RAD64 * degrees;
}

/// returns if a equals b, taking possible rounding errors into account
inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_F64)
{
    return (a + tolerance >= b) && (a - tolerance <= b);
}

/// returns if a equals b, taking possible rounding errors into account
inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_F32)
{
    return (a + tolerance >= b) && (a - tolerance <= b);
}

/// returns if a equals zero, taking rounding errors into account
inline bool isZero(const f64 a, const f64 tolerance = ROUNDING_ERROR_F64)
{
    return fabs(a) <= tolerance;
}

/// returns if a equals zero, taking rounding errors into account
inline bool isZero(const f32 a, const f32 tolerance = ROUNDING_ERROR_F32)
{
    return fabsf(a) <= tolerance;
}

template<class T>
inline const T & min(const T & a, const T & b)
{
    return a < b ? a : b;
}

template<class T>
inline const T & max(const T & a, const T & b)
{
    return a < b ? b : a;
}

/// \brief Clamps x to the given interval. If x is greater or lesser than
/// min or max, it will be backed to min or max.
/// \param x : value to clamp.
/// \param min
/// \param max
/// \return clamped value
template<class T>
inline const T & clamp(const T & x, const T & min, const T & max)
{
    if(x > max)
        return max;
    else if(x < min)
        return min;
    return x;
}

/// \brief Initializes the random seed used by math random functions
inline void randomSeed()
{
    std::srand(static_cast<u32>(std::time(nullptr)));
}

/// \brief Sets a specific seed for math random functions
inline void randomSeed(s32 seed)
{
    std::srand(seed);
}

/// \brief Generates random numbers in [min, max]
/// \note This function uses C rand().
/// \param min : minimum value. Assumed to be < max.
/// \param max : maximum value. Assumed to be > min.
/// \return pseudo-random number between min and max (included).
inline int rand(s32 min, s32 max)
{
    return min + std::rand() % (max - min + 1);
}

/// \brief Generates random numbers in [0.f, 1.f]
/// \note this is a quick-to-use function, but not a good random generator.
/// \return pseudo-random floating number between 0 and 1
inline f32 randf()
{
    return static_cast<f32>(std::rand()) / static_cast<f32>(RAND_MAX);
}

/// \brief Generates random numbers in [min, max].
/// \note this is a quick-to-use function, but not a good random generator.
/// \param min : minimum value. Assumed to be < max.
/// \param max : maximum value. Assumed to be > min.
/// \return pseudorandom number between min and max
inline f32 randf(f32 min, f32 max)
{
    return min + randf() * (max - min);
}

/// Returns the square of the given number
template <typename T>
inline f32 sq(T x)
{
    return x*x;
}

/// Returns the fractionnal part of a number
inline f32 frac(f32 x)
{
    return x-fabs(x);
}

/// Computes a value between 0 and len from x. Loops as x changes out of bounds.
inline f32 repeat(f32 x, f32 len)
{
    return len*frac(x/len);
}

/// Returns -1 if x is negative, 1 if x is positive, and 0 if x == 0
template <typename T>
inline T sign0(T x)
{
    if(x < 0)
        return -1;
    else if(x > 0)
        return 1;
    else
        return 0;
}

/// Interpolates src and dst with a linear factor
template <typename T>
inline T lerp(T src, T dst, f32 t)
{
    return src + (dst - src) * t;
}

/// Counts the number of bits set in an integer (human-readable version, not fastest)
/// See http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
/// Doing that the most efficient way depends on the platform.
/// Note: whenever possible, use std::bitset. It often has more optimized functions.
inline u32 bitcount(u32 value)
{
    u32 count = 0;
    while(value != 0) // until all bits are zero
    {
        if((value & 1) == 1) // check lower bit
            ++count;
        value >>= 1; // shift bits, removing lower bit
    }
    return count;
}

// TODO math: fast rsqrt for vector normalization

} // namespace math
} // namespace sn

#endif // __HEADER_SN_MATH__


