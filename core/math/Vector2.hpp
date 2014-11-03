/*
Vector2.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR2__
#define __HEADER_SN_VECTOR2__

#include <string>
#include <core/types.hpp>
#include <core/math/math.hpp>
#include <core/util/stringutils.hpp>

#ifdef SN_SFML
    #include <SFML/System.hpp>
#endif

namespace sn
{

// TODO Swizzle method? yx()

template <typename T>
class Vector2
{
public :

    T x;
    T y;

    /// Default constructor (null vector).
    Vector2() : x(0), y(0) {}

    /// Constructor with two different values
    Vector2(T nx, T ny) : x(nx), y(ny) {}

    /// Constructor with the same value for all elements
    /// Note : it is explicit to forbid aVector = aNumber;
    explicit Vector2(T n) : x(n), y(n) {}

    /// Copy constructor
    Vector2(const Vector2<T> & other) : x(other.x), y(other.y) {}

#ifdef SN_SFML
    Vector2(const sf::Vector2<T> other) : x(other.x), y(other.y) {}
#endif

    // operators

    Vector2<T> operator-() const
    {
        return Vector2<T>(-x, -y);
    }

    Vector2<T> & operator=(const Vector2<T> & other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vector2<T> operator+(const Vector2<T>& other) const
    {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> & operator+=(const Vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2<T> operator+(const T val) const
    {
        return Vector2<T>(x + val, y + val);
    }

    Vector2<T> & operator+=(const T val)
    {
        x += val;
        y += val;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T>& other) const
    {
        return Vector2<T>(x - other.x, y - other.y);
    }

    Vector2<T> & operator-=(const Vector2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2<T> operator-(const T val) const
    {
        return Vector2<T>(x - val, y - val);
    }

    Vector2<T> & operator-=(const T val)
    {
        x -=val;
        y -=val;
        return *this;
    }

    Vector2<T> operator*(const Vector2<T>& other) const
    {
        return Vector2<T>(x * other.x, y * other.y);
    }

    Vector2<T> & operator*=(const Vector2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vector2<T> operator*(const T v) const
    {
        return Vector2<T>(x * v, y * v);
    }

    Vector2<T> & operator*=(const T v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    Vector2<T> operator/(const Vector2<T>& other) const
    {
        return Vector2<T>(x / other.x, y / other.y);
    }

    Vector2<T> & operator/=(const Vector2<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vector2<T> operator/(const T v) const
    {
        T i = (T)1.0 / v;
        return Vector2<T>(x * i, y * i);
    }

    Vector2<T> & operator/=(const T v)
    {
        T i = (T)1.0 / v;
        x *= i;
        y *= i;
        return *this;
    }

    /// sort in order x, y. Difference must be above rounding tolerance.
    bool operator<(const Vector2<T>&other) const
    {
        return (x < other.x && !equals(x, other.x))
            || (equals(x, other.x)
                && y < other.y && !equals(y, other.y));
    }

    bool operator==(const Vector2<T>& other) const
    {
        return this->equals(other);
    }

    bool operator!=(const Vector2<T>& other) const
    {
        return !this->equals(other);
    }

    // functions

    /// Returns if this vector equals the other one,
    /// taking floating point rounding errors into account
    bool equals(const Vector2<T> & other) const
    {
        return equals(x, other.x) && equals(y, other.y);
    }

    Vector2<T> & set(const T nx, const T ny)
    {
        x = nx; y = ny;
        return *this;
    }

    Vector2<T> & set(const Vector2<T> & p)
    {
        x = p.x; y = p.y;
        return *this;
    }

    /// Get length of the vector.
    T length() const
    {
        return sqrt(x*x + y*y);
    }

    /// Get squared length of the vector.
    T getLengthSQ() const
    {
        return x*x + y*y;
    }

    /// Get the dot product with another vector.
    T dotProduct(const Vector2<T> & other) const
    {
        return x * other.x + y * other.y;
    }

    /// Get distance from another point.
    T getDistanceFrom(const Vector2<T>& other) const
    {
        return Vector2<T>(x - other.x, y - other.y).length();
    }

    /// Returns squared distance from another point.
    T getDistanceFromSQ(const Vector2<T>& other) const
    {
        return Vector2<T>(x - other.x, y - other.y).getLengthSQ();
    }

    /// Normalizes the vector.
    /// In case of the 0 vector the result is still 0, otherwise
    /// the length of the vector will be 1.
    Vector2<T> & normalize()
    {
        f64 lengthSQ = x*x + y*y;
        if(equals(lengthSQ, 0.0))
            return *this;
        const float l = 1.f / sqrtf(lengthSQ);

        x = (T)(x * l);
        y = (T)(y * l);
        return *this;
    }

    /// Sets the length of the vector to a new value
    Vector2<T> & setLength(T newlength)
    {
        normalize();
        return (*this *= newlength);
    }

    /// Inverts the vector.
    Vector2<T>& invert()
    {
        x *= -1;
        y *= -1;
        return *this;
    }

    //! rotates the point anticlockwise around a center by an amount of degrees.
    Vector2<T> & rotateBy(f64 degrees, const Vector2<T> & center = Vector2<T>())
    {
        degrees *= DEG2RAD64;
        const f64 cs = cos(degrees);
        const f64 sn = sin(degrees);

        x -= center.x;
        y -= center.y;

        set((T)(x*cs - y*sn), (T)(x*sn + y*cs));

        x += center.x;
        y += center.y;
        return *this;
    }

    /// Creates an interpolated vector between this vector and another vector.
    Vector2<T> getInterpolated(const Vector2<T>& other, f64 d) const
    {
        const f64 inv = 1.0 - d;
        return Vector2<T>(
            (T)(other.x * inv + x * d),
            (T)(other.y * inv + y * d));
    }

    /// Sets this vector to the linearly interpolated vector between a and b.
    Vector2<T>& interpolate(const Vector2<T>& a, const Vector2<T>& b, f64 d)
    {
        x = (T)((f64)b.x + ( ( a.x - b.x ) * d ));
        y = (T)((f64)b.y + ( ( a.y - b.y ) * d ));
        return *this;
    }

    std::string toString() const
    {
        return "(" + sn::toString(x)
            + ", " + sn::toString(y) + ")";
    }

};

/// Partial template specialization for integer vectors

template <>
inline Vector2<s32> Vector2<s32>::operator/(s32 val) const
{
    return Vector2<s32>(x/val, y/val);
}

template <>
inline Vector2<s32>& Vector2<s32>::operator/=(s32 val)
{
    x /= val; y /= val;
    return *this;
}

template <>
inline bool Vector2<s32>::operator<(const Vector2<s32> & other) const
{
    return (x < other.x && x != other.x)
        || ((x == other.x) && y < other.y && y != other.y);
}

template <>
inline bool Vector2<s32>::equals(const Vector2<s32> & other) const
{
    return x == other.x && y == other.y;
}

/// Stream and string utility

template <typename T>
std::ostream & operator<<(std::ostream & os, const Vector2<T> & vec)
{
    return os << vec.toString();
}

template <typename T>
std::string operator+(std::string const & a, Vector2<T> const & b)
{
    return a + b.toString();
}

/// Typedefs

typedef Vector2<s32> Vector2i;
typedef Vector2<u32> Vector2u;
typedef Vector2<f32> Vector2f;

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
            return v.x | (v.y << 16);
        }
    };

} // namespace std


//static T manhattanDistance(const Vector3<T> a, const Vector3<T> b)
//{
//	return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
//}

#endif // __HEADER_SN_VECTOR2__



