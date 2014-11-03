#ifndef __HEADER_SN_QUATERNION__
#define __HEADER_SN_QUATERNION__

#include <core/types.hpp>
#include <core/math/Vector3.hpp>

namespace sn
{

class SN_API Quaternion
{
public:

    //----------------------
    Quaternion(): m_w(1), m_x(0), m_y(0), m_z(0) {}

    //----------------------
    Quaternion(f32 w, f32 x, f32 y, f32 z): m_w(w), m_x(x), m_y(y), m_z(z) {}

    //----------------------
    Quaternion(const Quaternion & other):
        m_w(other.m_w),
        m_x(other.m_x),
        m_y(other.m_y),
        m_z(other.m_z)
    {}

    //----------------------
    Quaternion(f32 degreesX, f32 degreesY, f32 degreesZ)
    {
        setFromEuler(degreesX, degreesY, degreesZ);
    }

    //----------------------
    Quaternion(Vector3f eulerDegrees)
    {
        setFromEuler(eulerDegrees.x, eulerDegrees.y, eulerDegrees.z);
    }

    //----------------------
    inline bool operator==(const Quaternion& other) const
    {
        return m_w == other.m_w
            && m_x == other.m_x
            && m_y == other.m_y
            && m_z == other.m_z;
    }

    //----------------------
    inline bool operator!=(const Quaternion& other) const
    {
        return !((*this) == other);
    }

    //----------------------
    inline Quaternion & operator=(const Quaternion& other)
    {
        m_w = other.m_w;
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        return *this;
    }

    //----------------------
    inline Quaternion operator+(const Quaternion& other) const
    {
        Quaternion res;

        res.m_w = (other.m_w * m_w) - (other.m_x * m_x) - (other.m_y * m_y) - (other.m_z * m_z);
        res.m_x = (other.m_w * m_x) + (other.m_x * m_w) + (other.m_y * m_z) - (other.m_z * m_y);
        res.m_y = (other.m_w * m_y) + (other.m_y * m_w) + (other.m_z * m_x) - (other.m_x * m_z);
        res.m_z = (other.m_w * m_z) + (other.m_z * m_w) + (other.m_x * m_y) - (other.m_y * m_x);

        return res;
    }

    //----------------------
    inline Quaternion operator*(f32 s) const
    {
        return Quaternion(m_w*s, m_x*s, m_y*s, m_z*s);
    }

    //----------------------
    inline Quaternion operator*(const Quaternion & other) const
    {
        //r0*q0 - r1*q1 - r2*q2 + r3*q3
        //r0*q1 + r1*q0 - r2*q3 + r3*q2
        //r0*q2 + r1*q3 + r2*q0 - r3*q1
        //r0*q3 - r1*q2 + r2*q1 + r3*q0

        return Quaternion(
            m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z,
            m_w * other.m_x + m_x * other.m_w - m_y * other.m_z + m_z * other.m_y,
            m_w * other.m_y + m_x * other.m_z + m_y * other.m_w + m_z * other.m_x,
            m_w * other.m_z - m_x * other.m_y + m_y * other.m_x + m_z * other.m_w
        );
    }

    //----------------------
    inline Quaternion & operator*=(f32 s)
    {
        m_w *= s;
        m_x *= s;
        m_y *= s;
        m_z *= s;
        return *this;
    }

    //----------------------
    inline Vector3f operator*(const Vector3f & v) const
    {
        // nVidia SDK implementation

        Vector3f uv, uuv;
        Vector3f qvec(m_x, m_y, m_z);
        uv = qvec.crossProduct(v);
        uuv = qvec.crossProduct(uv);
        uv *= (2.0f * m_w);
        uuv *= 2.0f;

        return v + uv + uuv;
    }

    //----------------------
    inline Quaternion & operator*=(const Quaternion & other)
    {
        return (*this = other * (*this));
    }

    //----------------------
    inline void loadIdentity()
    {
        m_w = 1;
        m_x = 0;
        m_y = 0;
        m_z = 0;
    }

    //----------------------
    inline Quaternion & Quaternion::normalize()
    {
        const f32 n = m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w;

        if (n == 1)
            return *this;

        //n = 1.0f / sqrtf(n);
        return (*this *= (1.f / sqrt(n)));
    }

    //----------------------
    inline f32 Quaternion::dotProduct(const Quaternion & other) const
    {
        return (m_x * other.m_x) 
             + (m_y * other.m_y) 
             + (m_z * other.m_z) 
             + (m_w * other.m_w);
    }

    //----------------------
    inline void setFromEuler(f32 degreesX, f32 degreesY, f32 degreesZ)
    {
        f64 angle;

        angle = (degreesX * math::DEG2RAD) * 0.5;
        const f64 sr = sin(angle);
        const f64 cr = cos(angle);

        angle = (degreesY * math::DEG2RAD) * 0.5;
        const f64 sp = sin(angle);
        const f64 cp = cos(angle);

        angle = (degreesZ * math::DEG2RAD) * 0.5;
        const f64 sy = sin(angle);
        const f64 cy = cos(angle);

        const f64 cpcy = cp * cy;
        const f64 spcy = sp * cy;
        const f64 cpsy = cp * sy;
        const f64 spsy = sp * sy;

        m_x = (f32)(sr * cpcy - cr * spsy);
        m_y = (f32)(cr * spcy + sr * cpsy);
        m_z = (f32)(cr * cpsy - sr * spcy);
        m_w = (f32)(cr * cpcy + sr * spsy);

        normalize();
    }

    //----------------------
    inline Vector3f getEulerAngles() const
    {
        const f64 sqw = m_w * m_w;
        const f64 sqx = m_x * m_x;
        const f64 sqy = m_y * m_y;
        const f64 sqz = m_z * m_z;
        const f64 test = 2.0 * (m_y*m_w - m_x*m_z);

        Vector3f euler;

        if (math::equals(test, 1.0, 0.000001))
        {
            // heading = rotation about z-axis
            euler.z = (f32)(-2.0*atan2(m_x, m_w));
            // bank = rotation about x-axis
            euler.x = 0;
            // attitude = rotation about y-axis
            euler.y = (f32)(math::PI64 / 2.0);
        }
        else if (math::equals(test, -1.0, 0.000001))
        {
            // heading = rotation about z-axis
            euler.z = (f32)(2.0*atan2(m_x, m_w));
            // bank = rotation about x-axis
            euler.x = 0;
            // attitude = rotation about y-axis
            euler.y = (f32)(math::PI64 / -2.0);
        }
        else
        {
            // heading = rotation about z-axis
            euler.z = (f32)atan2(2.0 * (m_x*m_y + m_z*m_w), (sqx - sqy - sqz + sqw));
            // bank = rotation about x-axis
            euler.x = (f32)atan2(2.0 * (m_y*m_z + m_x*m_w), (-sqx - sqy + sqz + sqw));
            // attitude = rotation about y-axis
            euler.y = (f32)asin(math::clamp(test, -1.0, 1.0));
        }

        euler.x *= math::RAD2DEG;
        euler.y *= math::RAD2DEG;
        euler.z *= math::RAD2DEG;

        return euler;
    }

    //----------------------
    inline f32 getW() const { return m_w; }
    inline f32 getX() const { return m_x; }
    inline f32 getY() const { return m_y; }
    inline f32 getZ() const { return m_z; }

    //----------------------
    static inline Quaternion lerp(const Quaternion & q1, const Quaternion & q2, f32 t)
    {
        const f32 scale = 1.0f - t;
        return (q1*scale) + (q2*t);
    }

    //----------------------
    static inline Quaternion slerp(Quaternion q1, const Quaternion & q2, f32 t, f32 threshold)
    {
        f32 angle = q1.dotProduct(q2);

        // make sure we use the short rotation
        if (angle < 0.0f)
        {
            q1 *= -1.0f;
            angle *= -1.0f;
        }

        if (angle <= (1 - threshold)) // spherical interpolation
        {
            const f32 theta = acosf(angle);
            const f32 invsintheta = 1.0f / (sinf(theta));
            const f32 scale = sinf(theta * (1.0f - t)) * invsintheta;
            const f32 invscale = sinf(theta * t) * invsintheta;
            return (q1*scale) + (q2*invscale);
        }
        else // linear interploation
            return lerp(q1, q2, t);
    }

private:

    // Real part
    f32 m_w;

    // Imaginary part
    f32 m_x;
    f32 m_y;
    f32 m_z;

};

} // namespace sn

#endif // __HEADER_SN_QUATERNION__



