/*
Array2D.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ARRAY2D__
#define __HEADER_SN_ARRAY2D__

#include <cstring> // For memcpy and memset
#include <iostream>

#include <core/math/Vector2.h>
#include <core/util/Exception.h>
#include <core/util/Log.h>

namespace sn
{

/*
    Bi-dimensionnal flat array.
    Static because it is not resizeable,
    dynamic because it can be re-created without destroy its instance.
*/

template <typename T>
class Array2D
{
public :

    // creates an empty array
    Array2D() :
        m_data(nullptr),
        m_sizeX(0),
        m_sizeY(0)
    {}

    // creates an array with the specified size.
    // Note : data values are not initialized, use the fill() function if necessary.
    Array2D(u32 psizeX, u32 psizeY)
    {
        m_sizeX = psizeX;
        m_sizeY = psizeY;

        if(m_sizeX == 0 || m_sizeY == 0)
        {
            // Empty array
            m_data = nullptr;
        }
        else
        {
            // Allocate memory
            m_data = new T[area()];
        }
    }

    // creates an array with the specified size and value
    Array2D(u32 psizeX, u32 psizeY, const T & value)
    {
        m_sizeX = psizeX;
        m_sizeY = psizeY;

        if(m_sizeX == 0 || m_sizeY == 0)
        {
            // Empty array
            m_data = nullptr;
        }
        else
        {
            // Allocate memory
            m_data = new T[area()];
        }

        fill(value);
    }

    // creates an array as a copy from another
    Array2D(const Array2D & other) : m_data(nullptr)
    {
        copyFrom(other);
    }

    virtual ~Array2D()
    {
        clear();
    }

    Array2D<T> & operator=(const Array2D<T> & other)
    {
        copyFrom(other);
        return *this;
    }

    inline T * raw()
    {
        return m_data;
    }

    // Accesses directly the flat array.
    // i must be in [0, area()[.
    // i can be computed with getLocation(x,y).
    inline T & operator[](u32 i)
    {
        return m_data[i];
    }

    inline u32 getLocation(u32 x, u32 y) const
    {
        return m_sizeX * y + x;
    }

    // creates the buffer from the specified area.
    // old data is cleared.
    // Note : data values are not initialized, use the fill() function if necessary.
    void create(u32 psizeX, u32 psizeY)
    {
        // Delete old data
        if(m_data != nullptr)
        {
            delete[] m_data;
        }

        m_sizeX = psizeX;
        m_sizeY = psizeY;

        if(m_sizeX == 0 || m_sizeY == 0)
        {
            // Empty array
            SN_WARNING("Array2D::create received empty area (" << psizeX << ", " << psizeY << ")");
            m_data = nullptr;
        }
        else
        {
            // Allocate memory
            m_data = new T[area()];
        }
    }

    // Copies data from another array into this one.
    // Copy an empty array will clear this one.
    void copyFrom(const Array2D<T> & other)
    {
        if(other.empty())
        {
            clear();
        }
        else
        {
            create(other.sizeX(), other.sizeY());
            memcpy(m_data, other.m_data, area() * sizeof(T));
        }
    }

    // Returns the count of cells in the array
    inline u32 area() const
    {
        return m_sizeX * m_sizeY;
    }

    inline u32 sizeX() const { return m_sizeX; }
    inline u32 sizeY() const { return m_sizeY; }

    // clears the container
    void clear()
    {
        if(m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
            m_sizeX = 0;
            m_sizeY = 0;
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
        const u32 vol = area();
        for(u32 i = 0; i < vol; ++i)
            m_data[i] = val;
    }

    // get an element without position validation (it must be valid !)
    inline T getNoEx(s32 x, s32 y) const
    {
        return m_data[getLocation(x, y)];
    }

    // get an element
    T get(s32 x, s32 y) const
    {
        if(x < 0 || y < 0 || x >= m_sizeX || y >= m_sizeY)
            throw Exception("Array2D::get " + Vector2i(x,y));
        else
            return getNoEx(x, y);
    }

    // set an element without position validation (it must be valid !)
    inline void setNoEx(s32 x, s32 y, const T & value)
    {
        m_data[getLocation(x, y)] = value;
    }

    // Set an element
    void set(s32 x, s32 y, const T & value)
    {
        if(x < 0 || y < 0 || x >= static_cast<s32>(m_sizeX) || y >= static_cast<s32>(m_sizeY))
            throw Exception("Array2D::set " + toString(Vector2i(x,y)));
        else
            return setNoEx(x, y, value);
    }

    //
    // Convenience methods
    //

    // Tells if the given signed coordinates are valid in this array
    // (Convenience)
    inline bool contains(s32 x, s32 y) const
    {
        return x >= 0 && y >= 0 && x < static_cast<s32>(m_sizeX) && y < static_cast<s32>(m_sizeY);
    }

    // same as create(Area3D), with an additionnal parameter for default values
    // (Convenience)
    void create(u32 psizeX, u32 psizeY, const T & defaultValue)
    {
        create(psizeX, psizeY);
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
        return area() * elementByteCount();
    }

#ifdef __HEADER_SN_VECTOR2__

    // set an element
    // (convenience)
    inline void set(const Vector2i & pos, const T & value)
    {
        set(pos.x(), pos.y(), value);
    }

    // set an element without position validation (it must be valid !)
    // (convenience)
    inline void setNoEx(const Vector2i & pos, const T & value)
    {
        m_data[getLocation(pos.x(), pos.y())] = value;
    }

    // get an element
    // (convenience)
    inline T get(const Vector2i & pos) const
    {
        get(pos.x(), pos.y());
    }

    // get an element without position validation (it must be valid !)
    // (convenience)
    inline T getNoEx(const Vector2i & pos) const
    {
        return m_data[getLocation(pos.x(), pos.y())];
    }

#endif

private :

    T * m_data;	// linear data storage (nullptr if empty)
    u32 m_sizeX;
    u32 m_sizeY;

};

} // namespace sn

#endif // __HEADER_SN_ARRAY2D__



