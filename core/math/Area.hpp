#ifndef __HEADER_SN_MATH_AREA__
#define __HEADER_SN_MATH_AREA__

#include <core/math/Vector.hpp>

namespace sn
{

// Helpers
inline s32 getSizeFromMinMax(s32 min, s32 max) { return max - min + 1; }
inline f32 getSizeFromMinMax(f32 min, f32 max) { return max - min; }

// TODO Rename Area => Bounds?
template <typename T, unsigned int N>
class Area
{
public:
    //-----------------------------
    static Area fromPositionSize(const T x_, const T y_, const T w_, const T h_)
    {
        return Area(x_, y_, w_, h_);
    }

    //-----------------------------
    static Area fromPositionSize(const T x_, const T y_, const T z_, const T w_, const T h_, const T d_)
    {
        return Area(x_, y_, z_, w_, h_, d_);
    }

    //-----------------------------
	/// \brief Constructs a 2D area from an origin and max position, where max is included in bounds.
    static Area fromMinMax(T minX, T minY, T maxX, T maxY)
    {
        SN_STATIC_ASSERT(N == 2);
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
        return Area(minX, minY, 
			getSizeFromMinMax(minX,maxX), 
			getSizeFromMinMax(minY,maxY)
		);
    }

    //-----------------------------
	/// \brief Constructs a 3D area from an origin and max position, where max is included in bounds.
    static Area fromMinMax(T minX, T minY, T minZ, T maxX, T maxY, T maxZ)
    {
        SN_STATIC_ASSERT(N == 3);
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
        if (minZ > maxZ)
        {
            T temp = maxZ;
            maxZ = minZ;
            minZ = temp;
        }
        return Area(minX, minY, minZ, 
			getSizeFromMinMax(minX,maxX),
			getSizeFromMinMax(minY,maxY),
			getSizeFromMinMax(minZ,maxZ)
		);
    }

    //-----------------------------
    Area() {}

    //-----------------------------
    Area(const Area<T,N> & other):
        m_origin(other.m_origin),
        m_size(other.m_size)
    {}

    //-----------------------------
    Area(T px, T py, T width, T height) :
        m_origin(px, py),
        m_size(width, height)
    {
        SN_STATIC_ASSERT(N == 2);
    }

    //-----------------------------
    Area(T px, T py, T pz, T width, T height, T depth) :
        m_origin(px, py, pz),
        m_size(width, height, depth)
    {}

    //-----------------------------
    bool contains(T px, T py) const
    {
        SN_STATIC_ASSERT(N >= 2);
        return px >= x() 
            && py >= y() 
            && px < x() + width() 
            && py < y() + height();
    }

    //-----------------------------
    bool contains(T px, T py, T pz) const
    {
        SN_STATIC_ASSERT(N == 3);
        return px >= x() 
            && py >= y() 
            && pz >= z() 
            && px < x() + width() 
            && py < y + height() 
            && pz < z + depth();
    }

    //-----------------------------
    bool contains(const Vector<T, N> & other) const
    {
        for (u32 d = 0; d < N; ++d)
        {
            if (other[d] < m_origin[d] || other[d] >= m_origin[d] + m_size[d])
                return false;
        }
        return true;
    }

    //-----------------------------
    bool intersects(const Area<T, 2> & other) const
    {
        SN_STATIC_ASSERT(N >= 2);
        return maxX() >= other.minX()
            && maxY() >= other.minY() 
            && minX() <= other.maxX() 
            && minY() <= other.maxY();
    }

    //-----------------------------
    bool intersects(const Area<T, 3> & other) const
    {
        SN_STATIC_ASSERT(N == 3);
        return maxX() >= other.minX()
            && maxY() >= other.minY()
            && maxZ() >= other.minZ()
            && minX() <= other.maxX()
            && minY() <= other.maxY()
            && minZ() <= other.maxZ();
    }

    //-----------------------------
    bool intersectsQuad(const T px, const T py, const T ps) const
    {
        SN_STATIC_ASSERT(N >= 2);
        return maxX() >= px
            && maxY() >= py 
            && minX() <= px + ps 
            && minY() <= py + ps;
    }

    //-----------------------------
    bool intersectsQuad(const T px, const T py, const T pz, const T ps) const
    {
        SN_STATIC_ASSERT(N >= 3);
        return maxX() >= px
            && maxY() >= py
            && maxZ() >= py
            && minX() <= px + ps
            && minY() <= px + ps
            && minZ() <= py + ps;
    }

    //-----------------------------
    void addPoint(const T px, const T py)
    {
        SN_STATIC_ASSERT(N >= 2);

        if (px < m_origin.x()) m_origin.x() = px;
        if (py < m_origin.y()) m_origin.y() = py;

        if (px - m_origin.x() >= m_size.x()) m_size.x() = px - m_origin.x();
        if (py - m_origin.y() >= m_size.y()) m_size.y() = py - m_origin.y();
    }

    //-----------------------------
    void addPoint(const T px, const T py, const T pz)
    {
        SN_STATIC_ASSERT(N >= 3);
        
        if (px < m_origin.x()) m_origin.x() = px;
        if (py < m_origin.y()) m_origin.y() = py;
        if (pz < m_origin.z()) m_origin.z() = pz;

        if (px - m_origin.x() >= m_size.x()) m_size.x() = px - m_origin.x();
        if (py - m_origin.y() >= m_size.y()) m_size.y() = py - m_origin.y();
        if (pz - m_origin.z() >= m_size.z()) m_size.z() = pz - m_origin.z();
    }

    //-----------------------------
    void addPoint(const Vector<T, N> & p)
    {
        for (u32 d = 0; d < N; ++d)
        {
            if (p[d] < m_origin[d])
                m_origin[d] = px;
            
            if (p[d] - m_origin[d] >= m_size[d])
                m_size[d] = p[d] - m_origin[d];
        }
    }

    //-----------------------------
    std::string toString() const
    {
        std::stringstream ss;
        ss << "(origin=" << sn::toString(m_origin) << ", size=" << sn::toString(m_size) << ")";
        return ss.str();
    }

    //-----------------------------
    Vector<T, N> & origin() { return m_origin; }
    Vector<T, N> & size() { return m_size; }

    //-----------------------------
    const Vector<T, N> & origin() const { return m_origin; }
    const Vector<T, N> & size() const { return m_size; }

    //-----------------------------
    Vector<T, N> getMin() const { return m_origin; }
    Vector<T, N> getMax() const { return m_origin + m_size; }

    //-----------------------------
    T & x() { SN_STATIC_ASSERT(N >= 1); return m_origin.x(); }
    T & y() { SN_STATIC_ASSERT(N >= 2); return m_origin.y(); }
    T & z() { SN_STATIC_ASSERT(N >= 3); return m_origin.z(); }

    //-----------------------------
    const T & x() const { SN_STATIC_ASSERT(N >= 1); return m_origin.x(); }
    const T & y() const { SN_STATIC_ASSERT(N >= 2); return m_origin.y(); }
    const T & z() const { SN_STATIC_ASSERT(N >= 3); return m_origin.z(); }

    //-----------------------------
    T & width()  { SN_STATIC_ASSERT(N >= 1); return m_size.x(); }
    T & height() { SN_STATIC_ASSERT(N >= 2); return m_size.y(); }
    T & depth()  { SN_STATIC_ASSERT(N >= 3); return m_size.z(); }

    //-----------------------------
    const T & width()  const { SN_STATIC_ASSERT(N >= 1); return m_size.x(); }
    const T & height() const { SN_STATIC_ASSERT(N >= 2); return m_size.y(); }
    const T & depth()  const { SN_STATIC_ASSERT(N >= 3); return m_size.z(); }

    //-----------------------------
    T minX() const { SN_STATIC_ASSERT(N >= 1); return m_origin.x(); }
    T minY() const { SN_STATIC_ASSERT(N >= 2); return m_origin.y(); }
    T minZ() const { SN_STATIC_ASSERT(N >= 3); return m_origin.z(); }

    //-----------------------------
    T maxX() const { SN_STATIC_ASSERT(N >= 1); return m_origin.x() + m_size.x(); }
    T maxY() const { SN_STATIC_ASSERT(N >= 2); return m_origin.y() + m_size.y(); }
    T maxZ() const { SN_STATIC_ASSERT(N >= 3); return m_origin.z() + m_size.z(); }

private:
    Vector<T, N> m_origin;
    Vector<T, N> m_size;

};

//template <typename T>
//bool operator!=(const TRect<T> & a, const TRect<T> & b)
//{
//    return a.x != b.x
//        && a.y != b.y
//        && a.width != b.width
//        && a.height != b.height;
//}

typedef Area<s32, 3> IntAABB;
typedef Area<f32, 3> FloatAABB;

} // namespace sn

#endif // __HEADER_SN_MATH_AREA__
