/*
Array3D.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ARRAY3D__
#define __HEADER_SN_ARRAY3D__

#include <cstring> // For memcpy and memset
#include <iostream>

#include <core/math/Vector3.hpp>
#include <core/util/Exception.hpp>
#include <core/util/Log.hpp>

namespace sn
{

/*
    Tri-dimensionnal array.
*/

template <typename T>
class Array3D
{
public :

    // creates an empty array
    Array3D()
        : m_data(nullptr),
        m_sizeX(0),
        m_sizeY(0),
        m_sizeZ(0)
    {}

    // creates an array with the specified size.
    // Note : data values are not initialized, use the fill() function if necessary.
    Array3D(u32 sizeX, u32 sizeY, u32 sizeZ)
    {
        create(sizeX, sizeY, sizeZ);
    }

    // creates an array with the specified size and value
    Array3D(u32 sizeX, u32 sizeY, u32 sizeZ, const T & value)
    {
        create(sizeX, sizeY, sizeZ, value);
    }

    // creates an array as copy from another
    Array3D(const Array3D & other)
    {
        copyFrom(other);
    }

    virtual ~Array3D()
    {
        clear();
    }

    Array3D<T> & operator=(const Array3D<T> & other)
    {
        copyFrom(other);
        return *this;
    }

    // Access the inner buffer directly. Use getLocation(x,y,z) to get the index.
    inline T & operator[](u32 i)
    {
        return m_data[i];
    }

    inline u32 getLocation(u32 x, u32 y, u32 z) const
    {
        return m_sizeX * (z * m_sizeY + y) + x;
    }

    // creates the buffer from the specified area.
    // old data is cleared.
    // Note : data values are not initialized, use the fill() function if necessary.
    void create(u32 sizeX, u32 sizeY, u32 sizeZ)
    {
        // Delete old data
        if (m_data != nullptr)
        {
            clear();
        }

        // Check if the given area is correct
        if(sizeX == 0 || sizeY == 0 || sizeZ == 0)
        {
            SN_WARNING("Array3D::create received empty area (" << sizeX << ", " << sizeY << ", " << sizeZ << ")");
            return;
        }

        // Allocate memory
        m_sizeX = sizeX;
        m_sizeY = sizeY;
        m_sizeZ = sizeZ;
        m_data = new T[volume()];
    }

    // Copies data from another array into this one.
    // Copy an empty array will clear this one.
    void copyFrom(const Array3D<T> & other)
    {
        if(other.empty())
            clear();
        else
        {
            create(other.sizeX(), other.sizeY(), other.sizeZ());
            memcpy(m_data, other.m_data, volume() * sizeof(T));
        }
    }

    // Returns the count of cells in the array
    inline u32 volume() const
    {
        return m_sizeX * m_sizeY * m_sizeZ;
    }

    inline u32 sizeX() const { return m_sizeX; }
    inline u32 sizeY() const { return m_sizeY; }
    inline u32 sizeZ() const { return m_sizeZ; }

    // clears the container
    void clear()
    {
        if(m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
            m_sizeX = 0;
            m_sizeY = 0;
            m_sizeZ = 0;
        }
    }

    // Tests if there is space allocated in the array
    inline bool empty() const
    {
        return m_data == nullptr;
    }

    // fills the buffer with the same value
    void fill(const T & val)
    {
        // TODO optimize for byte-size types (memset)
        const u32 vol = volume();
        for(u32 i = 0; i < vol; ++i)
            m_data[i] = val;
    }

    // get an element without position validation (it must be valid !)
    inline T getNoEx(s32 x, s32 y, s32 z) const
    {
        return m_data[getLocation(x, y, z)];
    }

    // get an element
    T get(s32 x, s32 y, s32 z) const throw(Exception)
    {
        if(x < 0 || y < 0 || z < 0 || x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ)
            throw Exception("Array3D::get " + Vector3i(x,y,z));
        else
            return getNoEx(x, y, z);
    }

    // set an element without position validation (it must be valid !)
    inline void setNoEx(s32 x, s32 y, s32 z, const T & value)
    {
        m_data[getLocation(x, y, z)] = value;
    }

    // Set an element
    void set(s32 x, s32 y, s32 z, const T & value) throw(Exception)
    {
        if(x < 0 || y < 0 || z < 0 || x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ)
            throw Exception("Array3D::set " + Vector3i(x,y,z));
        else
            return setNoEx(x, y, z);
    }

    //
    // Convenience methods
    //

    // same as create(Area3D), with an additionnal parameter for default values
    // (Convenience)
    void create(u32 sizeX, u32 sizeY, u32 sizeZ, const T & defaultValue)
    {
        create(sizeX, sizeY, sizeZ);
        fill(defaultValue);
    }

    // Returns the data size of an element in bytes
    // (convenience)
    inline u32 elementByteCount() const
    {
        return sizeof(T);
    }

    // Returns the size of the data contained in the array in bytes
    // (convenience)
    inline u32 byteCount() const
    {
        return volume() * elementByteCount();
    }

    // set an element
    // (convenience)
    inline void set(const Vector3i & pos, const T & value) throw(Exception)
    {
        set(pos.x, pos.y, pos.z, value);
    }

    // set an element without position validation (it must be valid !)
    // (convenience)
    inline void setNoEx(const Vector3i & pos, const T & value)
    {
        m_data[getLocation(pos.x, pos.y, pos.z)] = value;
    }

    // get an element
    // (convenience)
    inline T get(const Vector3i & pos) const throw(Exception)
    {
        get(pos.x, pos.y, pos.y);
    }

    // get an element without position validation (it must be valid !)
    // (convenience)
    inline T getNoEx(const Vector3i & pos) const
    {
        return m_data[getLocation(pos.x, pos.y, pos.z)];
    }

private :

    T * m_data;	// linear data storage (nullptr if empty)
    u32 m_sizeX;
    u32 m_sizeY;
    u32 m_sizeZ;

};

} // namespace sn

#endif // __HEADER_SN_ARRAY3D__



