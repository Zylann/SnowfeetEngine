/*
Vector.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VECTOR__
#define __HEADER_SN_VECTOR__

#include <cstring>
#include <sstream>
#include <core/math/math.hpp>
#include <core/util/assert.hpp>

namespace sn
{

/// \brief Generic implementation of any geometric vector type
template <typename T, u32 N>
class Vector
{
public:

    //------------------------------------
    // Constructors
    //------------------------------------

    Vector()
    {
        memset(m_v, 0, N * sizeof(T));
    }

    Vector(const Vector<T, N> & other)
    {
        memcpy(m_v, other.m_v, N * sizeof(T));
    }

    // Conversion
    template <typename U>
    Vector(const Vector<U, N> & other)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] = static_cast<T>(other[i]);
    }

    explicit Vector(const T * values)
    {
        memcpy(m_v, values, N * sizeof(T));
    }

    Vector(T x, T y)
    {
        SN_STATIC_ASSERT(N == 2);
        m_v[0] = x;
        m_v[1] = y;
    }

    Vector(T x, T y, T z)
    {
        SN_STATIC_ASSERT(N == 3);
        m_v[0] = x;
        m_v[1] = y;
        m_v[2] = z;
    }

    Vector(T x, T y, T z, T w)
    {
        SN_STATIC_ASSERT(N == 4);
        m_v[0] = x;
        m_v[1] = y;
        m_v[2] = z;
        m_v[3] = w;
    }

    //------------------------------------
    // Operators
    //------------------------------------

    inline T & operator[](const u32 i)
    {
        return m_v[i];
    }

    inline const T & operator[](const u32 i) const
    {
        return m_v[i];
    }

    inline Vector<T,N> & operator=(const Vector<T, N> & other)
    {
        memcpy(m_v, other.m_v, N * sizeof(T));
        return *this;
    }

    inline bool operator==(const Vector<T, N> & other) const
    {
        for (u32 i = 0; i < N; ++i)
        {
            if (m_v[i] != other.m_v[i])
                return false;
        }
        return true;
    }

    inline bool operator!=(const Vector<T, N> & other) const
    {
        return !(*this == other);
    }

    inline Vector<T,N> operator-() const
    {
        Vector<T, N> res;
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] = -m_v[i];
        return res;
    }

    inline Vector<T,N> operator+(const Vector<T,N>& other) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] += other.m_v[i];
        return res;
    }

    inline Vector<T,N> & operator+=(const Vector<T,N>& other)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] += other.m_v[i];
        return *this;
    }

    inline Vector<T,N> operator-(const Vector<T,N>& other) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] -= other.m_v[i];
        return res;
    }

    inline Vector<T,N> & operator-=(const Vector<T,N>& other)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] -= other.m_v[i];
        return *this;
    }

    inline Vector<T,N> operator*(const Vector<T,N>& other) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] *= other.m_v[i];
        return res;
    }

    inline Vector<T,N> & operator*=(const Vector<T,N>& other)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] *= other.m_v[i];
        return *this;
    }

    inline Vector<T,N> operator*(const T v) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] *= v;
        return res;
    }

    inline Vector<T,N> & operator*=(const T v)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] *= v;
        return *this;
    }

    inline Vector<T,N> operator/(const Vector<T,N>& other) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] /= other.m_v[i];
        return res;
    }

    inline Vector<T,N> & operator/=(const Vector<T,N>& other)
    {
        for (u32 i = 0; i < N; ++i)
            m_v[i] /= other.m_v[i];
        return *this;
    }

    inline Vector<T,N> operator/(const T v) const
    {
        Vector<T, N> res(m_v);
        for (u32 i = 0; i < N; ++i)
            res.m_v[i] /= v;
        return res;
    }

    inline Vector<T,N> & operator/=(const T v)
    {
        T i = (T)1.0 / v;
        for (u32 i = 0; i < N; ++i)
            m_v[i] *= v;
        return *this;
    }

    /// sort in order x, y. Difference must be above rounding tolerance.
    inline bool operator<(const Vector<T,N>&other) const
    {
        for (u32 i = 0; i < N; ++i)
        {
            if (m_v[i] < other.m_v[i])
                return true;
            else if (m_v[i] != other.m_v[i])
                return false;
        }
        return false;
    }
    
    //------------------------------------
    // Methods
    //------------------------------------

    inline T getLengthSQ() const
    {
        T s = 0;
        for (u32 i = 0; i < N; ++i)
            s += math::sq(m_v[i]);
        return s;
    }

    inline f32 getLength() const
    {
        return sqrt(getLengthSQ());
    }

    inline Vector<T,N> & setLength(T newlength)
    {
        *this = normalize();
        return (*this *= newlength);
    }

    inline Vector<T,N> & normalize()
    {
        T lenSQ = getLengthSQ();
        if (lenSQ == 0)
            return *this;

        const f32 len = 1.f / sqrtf(lenSQ);

        for (u32 i = 0; i < N; ++i)
            m_v[i] = (T)(m_v[i] * len);

        return *this;
    }

    inline T dotProduct(const Vector<T, N> & other)
    {
        T p = 0;
        for (u32 i = 0; i < N; ++i)
            p += m_v[i] * other.m_v[i];
        return p;
    }

    //------------------------------------
    // Properties
    //------------------------------------

    inline T & x() { SN_STATIC_ASSERT(N > 0); return m_v[0]; }
    inline T & y() { SN_STATIC_ASSERT(N > 1); return m_v[1]; }
    inline T & z() { SN_STATIC_ASSERT(N > 2); return m_v[2]; }
    inline T & w() { SN_STATIC_ASSERT(N > 3); return m_v[3]; }

    inline const T & x() const { SN_STATIC_ASSERT(N > 0); return m_v[0]; }
    inline const T & y() const { SN_STATIC_ASSERT(N > 1); return m_v[1]; }
    inline const T & z() const { SN_STATIC_ASSERT(N > 2); return m_v[2]; }
    inline const T & w() const { SN_STATIC_ASSERT(N > 3); return m_v[3]; }

    T m_v[N];
};

template <typename T, u32 N>
std::string toString(const Vector<T, N> & v)
{
    std::stringstream ss;
    ss << "{" << v[0];
    for (u32 i = 1; i < N; ++i)
    {
        ss << ", " << v[i];
    }
    ss << "}";
    return ss.str();
}

} // namespace sn

#endif // __HEADER_SN_VECTOR__



