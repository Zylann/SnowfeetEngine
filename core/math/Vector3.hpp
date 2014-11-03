/*
Vector3.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR3__
#define __HEADER_SN_VECTOR3__

#include <string>
#include <cstdlib>
#include <core/types.hpp>
#include <core/math/math.hpp>
#include <core/util/stringutils.hpp>

#ifdef SN_SFML
    #include <SFML/System.hpp>
#endif

namespace sn
{
// TODO implement operators outside the class
// TODO Swizzle methods? xzy(), yzx(), yxz(), zxy(), zyx()

template <typename T>
class Vector3
{
public :

    T x;
    T y;
    T z;

    /// Default constructor (null vector).
    Vector3() : x(0), y(0), z(0) {}

    /// Constructor with three different values
    Vector3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}

    /// Constructor with the same value for all elements
    /// Note : it is explicit to forbid aVector = aNumber;
    explicit Vector3(T n) : x(n), y(n), z(n) {}

    /// Copy constructor
    Vector3(const Vector3<T> & other) : x(other.x), y(other.y), z(other.z) {}

#ifdef SN_SFML
    Vector3(const sf::Vector3<T> other) : x(other.x), y(other.y), z(other.z) {}
#endif

    // operators

    Vector3<T> operator-() const
    {
        return Vector3<T>(-x, -y, -z);
    }

    Vector3<T> & operator=(const Vector3<T> & other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3<T> operator+(const Vector3<T>& other) const
    {
        return Vector3<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector3<T> & operator+=(const Vector3<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3<T> operator+(const T val) const
    {
        return Vector3<T>(x + val, y + val, z + val);
    }

    Vector3<T> & operator+=(const T val)
    {
        x += val;
        y += val;
        z += val;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T>& other) const
    {
        return Vector3<T>(x - other.x, y - other.y, z - other.z);
    }

    Vector3<T> & operator-=(const Vector3<T>& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3<T> operator-(const T val) const
    {
        return Vector3<T>(x - val, y - val, z - val);
    }

    Vector3<T> & operator-=(const T val)
    {
        x -=val;
        y -=val;
        z -=val;
        return *this;
    }

    Vector3<T> operator*(const Vector3<T>& other) const
    {
        return Vector3<T>(x * other.x, y * other.y, z * other.z);
    }

    Vector3<T> & operator*=(const Vector3<T>& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3<T> operator*(const T v) const
    {
        return Vector3<T>(x * v, y * v, z * v);
    }

    Vector3<T> & operator*=(const T v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    Vector3<T> operator/(const Vector3<T>& other) const
    {
        return Vector3<T>(x / other.x, y / other.y, z / other.z);
    }

    Vector3<T> & operator/=(const Vector3<T>& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3<T> operator/(const T v) const
    {
        T i = (T)1.0 / v;
        return Vector3<T>(x * i, y * i, z * i);
    }

    Vector3<T> & operator/=(const T v)
    {
        T i = (T)1.0 / v;
        x *= i;
        y *= i;
        z *= i;
        return *this;
    }

    /// sort in order x, y, z. Difference must be above rounding tolerance.
    bool operator<(const Vector3<T>&other) const
    {
        return (x < other.x && !math::equals(x, other.x))
            || (math::equals(x, other.x)
                && y < other.y && !math::equals(y, other.y))
            || (math::equals(x, other.x)
                && math::equals(y, other.y)
                && z < other.z && !math::equals(z, other.z));
    }

    bool operator==(const Vector3<T>& other) const
    {
        return this->equals(other);
    }

    bool operator!=(const Vector3<T>& other) const
    {
        return !this->equals(other);
    }

    // functions

    /// Returns if this vector equals the other one,
    /// taking floating point rounding errors into account
    bool equals(const Vector3<T> & other) const
    {
        return math::equals(x, other.x) &&
            math::equals(y, other.y) &&
            math::equals(z, other.z);
    }

    Vector3<T> & set(const T nx, const T ny, const T nz)
    {
        x = nx; y = ny; z = nz;
        return *this;
    }

    Vector3<T> & set(const Vector3<T> & p)
    {
        x = p.x; y = p.y; z = p.z;
        return *this;
    }

    /// Get length of the vector.
    float getLength() const
    {
        return sqrt(x*x + y*y + z*z);
    }

    /// Get squared length of the vector.
    T getLengthSQ() const
    {
        return x*x + y*y + z*z;
    }

    /// Get the dot product with another vector.
    T dotProduct(const Vector3<T> & other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    /// Get distance from another point.
    float getDistanceFrom(const Vector3<T> & other) const
    {
        return Vector3<T>(x - other.x, y - other.y, z - other.z).getLength();
    }

    /// Returns squared distance from another point.
    T getDistanceFromSQ(const Vector3<T> & other) const
    {
        return Vector3<T>(x - other.x, y - other.y, z - other.z).getLengthSQ();
    }

    /// Calculates the cross product with another vector.
    Vector3<T> crossProduct(const Vector3<T> & p) const
    {
        return Vector3<T>(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }

    /// Normalizes the vector.
    /// In case of the 0 vector the result is still 0, otherwise
    /// the length of the vector will be 1.
    Vector3<T> & normalize()
    {
        f64 length = x*x + y*y + z*z;
        if(math::equals(length, 0.0))
            return *this;
        length = static_cast<f64>(1.0 / sqrt(length));

        x = (T)(x * length);
        y = (T)(y * length);
        z = (T)(z * length);
        return *this;
    }

    /// Sets the length of the vector to a new value
    Vector3<T> & setLength(T newlength)
    {
        normalize();
        return (*this *= newlength);
    }

    /// Inverts the vector.
    Vector3<T>& invert()
    {
        x *= -1;
        y *= -1;
        z *= -1;
        return *this;
    }

    /// Rotates the vector by a specified number of degrees
    /// around the y axis and the specified center.
    void rotateXZBy(f64 degrees, const Vector3<T>& center = Vector3<T>())
    {
        degrees *= math::DEG2RAD64;
        f64 cs = cos(degrees);
        f64 sn = sin(degrees);
        x -= center.x;
        z -= center.z;
        set((T)(x*cs - z*sn), y, (T)(x*sn + z*cs));
        x += center.x;
        z += center.z;
    }

    /// Rotates the vector by a specified number of degrees
    /// around the z axis and the specified center.
    void rotateXYBy(f64 degrees, const Vector3<T>& center=Vector3<T>())
    {
        degrees *= math::DEG2RAD64;
        f64 cs = cos(degrees);
        f64 sn = sin(degrees);
        x -= center.x;
        y -= center.y;
        set((T)(x*cs - y*sn), (T)(x*sn + y*cs), z);
        x += center.x;
        y += center.y;
    }

    /// Rotates the vector by a specified number of degrees
    /// around the x axis and the specified center.
    void rotateYZBy(f64 degrees, const Vector3<T>& center=Vector3<T>())
    {
        degrees *= math::DEG2RAD64;
        f64 cs = cos(degrees);
        f64 sn = sin(degrees);
        z -= center.z;
        y -= center.y;
        set(x, (T)(y*cs - z*sn), (T)(y*sn + z*cs));
        z += center.z;
        y += center.y;
    }

    /// Creates an interpolated vector between this vector and another vector.
    Vector3<T> getInterpolated(const Vector3<T>& other, f64 d) const
    {
        const f64 inv = 1.0 - d;
        return Vector3<T>(
            (T)(other.x * inv + x * d),
            (T)(other.y * inv + y * d),
            (T)(other.z * inv + z * d));
    }

    /// Sets this vector to the linearly interpolated vector between a and b.
    Vector3<T> & interpolate(const Vector3<T> & a, const Vector3<T> & b, f64 d)
    {
        x = (T)((f64)b.x + ( ( a.x - b.x ) * d ));
        y = (T)((f64)b.y + ( ( a.y - b.y ) * d ));
        z = (T)((f64)b.z + ( ( a.z - b.z ) * d ));
        return *this;
    }

    /// Fills an array of 4 values with the vector data (useful for shader vars).
    /// the 4th value is always 0.
    void getAs4Values(T * array) const
    {
        array[0] = x;
        array[1] = y;
        array[2] = z;
        array[3] = 0;
    }

    std::string toString() const
    {
        return "(" + sn::toString(x)
            + ", " + sn::toString(y)
            + ", " + sn::toString(z) + ")";
    }

};

/// Partial template specialization for integer vectors

template <>
inline Vector3<s32> Vector3<s32>::operator/(s32 val) const
{
    return Vector3<s32>(x/val, y/val, z/val);
}

template <>
inline Vector3<s32>& Vector3<s32>::operator/=(s32 val)
{
    x /= val; y /= val; z /= val;
    return *this;
}

template <>
inline bool Vector3<s32>::operator<(const Vector3<s32> & other) const
{
    return (x < other.x && x != other.x)
        || ((x == other.x) && y < other.y && y != other.y)
        || ((x == other.x) && y == other.y && z < other.z && z != other.z);
}

template <>
inline bool Vector3<s32>::equals(const Vector3<s32> & other) const
{
    return x == other.x && y == other.y && z == other.z;
}

/// Stream and string utility

template <typename T>
std::ostream & operator<<(std::ostream & os, const Vector3<T> & vec)
{
    return os << vec.toString();
}

template <typename T>
std::wostream & operator<<(std::wostream & wos, const Vector3<T> & vec)
{
    std::string str = vec.toString();
    for(unsigned int i = 0; i < str.size(); ++i)
        wos.put(str[i]);
    return wos;
}

template <typename T>
std::string operator+(std::string const & a, Vector3<T> const & b)
{
    return a + b.toString();
}

/// Typedefs

typedef Vector3<s32> Vector3i;
typedef Vector3<f32> Vector3f;


} // namespace sn

#endif // __HEADER_SN_VECTOR3__



