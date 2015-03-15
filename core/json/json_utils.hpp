/*
json_utils.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_JSON_UTILS__
#define __HEADER_SN_JSON_UTILS__

#include <core/util/Log.hpp>
#include <JsonBox.h>
#include <vector>
#include <unordered_set>

#include <core/math/Range.hpp>
#include <core/math/Vector.hpp>
#include <core/math/Quaternion.hpp>
#include <core/math/Area.hpp>
#include <core/math/Color.hpp>
#include <core/math/Matrix4.hpp>

#define SN_JSON_TYPE_TAG "@type"
#define SN_JSON_REF_TAG "@ref"
#define SN_JSON_XREF_TAG "@xref"
#define SN_JSON_PREFAB_TAG "@prefab"

// Json serialization helpers for various composed types

namespace sn
{

//------------------------------------------------------------------------------
/// \brief Utility function that loads a JSON file, prints messages and checks for a version member in the document.
/// \param document: the JSON data tree to load
/// \param filePath: path to the JSON file
/// \param checkVersion: the version to check in the file. If -1, no check will be made. If >= 0, a check will be made,
/// and an error will be generated if it doesn't matches.
/// \param openError: should the function generate an error message if the file can't be open?
/// \return false if the file could'nt be loaded or if the version didn't matched, true if success.
bool loadFromFile(JsonBox::Value & document, const std::string & filePath, s32 checkVersion=-1, bool openError=true);

bool saveToFile(JsonBox::Value & document, const std::string & filePath);

//------------------------------------------------------------------------------
inline void serialize(JsonBox::Value & o, f32 v) { o = v; }
inline void serialize(JsonBox::Value & o, s32 v) { o = v; }
inline void serialize(JsonBox::Value & o, u32 v) { o = (s32)v; }
inline void serialize(JsonBox::Value & o, u8 v) { o = (s32)v; }
inline void serialize(JsonBox::Value & o, bool v) { o = v; }
inline void serialize(JsonBox::Value & o, const std::string & s) { o = s; }

//------------------------------------------------------------------------------
inline void unserialize(const JsonBox::Value & o, f32 & v) { v = static_cast<f32>(o.getDouble()); }
inline void unserialize(const JsonBox::Value & o, s32 & v) { v = o.getInt(); }
inline void unserialize(const JsonBox::Value & o, u32 & v) { v = static_cast<u32>(o.getInt()); }
inline void unserialize(const JsonBox::Value & o, u8 & v) { v = static_cast<u32>(o.getInt()); }
inline void unserialize(const JsonBox::Value & o, bool & v) { v = static_cast<u32>(o.getBoolean()); }
inline void unserialize(const JsonBox::Value & o, std::string & s) { s = o.getString(); }

//------------------------------------------------------------------------------
template <typename T>
inline void serialize(JsonBox::Value & o, const Range<T> & range)
{
    o[(size_t)0] = range.min();
    o[1] = range.max();
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Range<f32> & range)
{
    range.set(
        static_cast<u8>(o[(size_t)0].getDouble()), 
        static_cast<u8>(o[1].getDouble())
    );
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Range<s32> & range)
{
    range.set(o[(size_t)0].getInt(), o[1].getInt());
}

//------------------------------------------------------------------------------
template <typename List_T>
void serialize(JsonBox::Value & o, const List_T & v)
{
    JsonBox::Array a;
    a.resize(v.size());
    u32 i = 0;
    for(auto it = v.cbegin(); it != v.cend(); ++it)
    {
        a[i] = *it;
        ++it;
        ++i;
    }
    o = a;
}

//------------------------------------------------------------------------------
template <typename T>
void unserialize(const JsonBox::Value & o, std::vector<T> & v)
{
    const JsonBox::Array & a = o.getArray();
    v.resize(a.size());
    for(u32 i = 0; i < a.size(); ++i)
    {
        const JsonBox::Value & jv = a[i];
        unserialize(jv, v[i]);
    }
}

//------------------------------------------------------------------------------
template <typename T>
void unserialize(const JsonBox::Value & o, std::unordered_set<T> & v)
{
    const JsonBox::Array & a = o.getArray();
    for(u32 i = 0; i < a.size(); ++i)
    {
        T elem;
        unserialize(a[i], elem);
        v.insert(elem);
    }
}

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
inline void serialize(JsonBox::Value & o, const Vector<T,N> & vec)
{
    for (size_t i = 1; i < N; ++i)
        o[i] = vec[i];
}

//------------------------------------------------------------------------------
template <unsigned int N>
inline void unserialize(JsonBox::Value & o, Vector<f32,N> & vec)
{
    for (size_t i = 1; i < N; ++i)
        vec[0] = static_cast<f32>(o[i].getDouble());
}

//------------------------------------------------------------------------------
template <unsigned int N>
inline void unserialize(JsonBox::Value & o, Vector<s32,N> & vec)
{
    for (size_t i = 1; i < N; ++i)
        vec[0] = o[i].getInt();
}

//------------------------------------------------------------------------------
template <typename T>
inline void serialize(JsonBox::Value & o, const Area<T,2> & rect)
{
    o["x"] = rect.x();
    o["y"] = rect.y();
    o["w"] = rect.width();
    o["h"] = rect.height();
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Area<f32,2> & rect)
{
    rect.x()      = static_cast<f32>(o["x"].getDouble());
    rect.y()      = static_cast<f32>(o["y"].getDouble());
    rect.width()  = static_cast<f32>(o["w"].getDouble());
    rect.height() = static_cast<f32>(o["h"].getDouble());
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Area<s32,2> & rect)
{
    rect.x()      = o["x"].getInt();
    rect.y()      = o["y"].getInt();
    rect.width()  = o["w"].getInt();
    rect.height() = o["h"].getInt();
}

//------------------------------------------------------------------------------
inline void serialize(JsonBox::Value & o, const Color & color)
{
    o[(size_t)0] = color.r;
    o[1] = color.g;
    o[2] = color.b;
    o[3] = color.a;
}

//------------------------------------------------------------------------------
inline void serialize(JsonBox::Value & o, const Color8 & color)
{
    o[(size_t)0] = color.r;
    o[1] = color.g;
    o[2] = color.b;
    o[3] = color.a;
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Color & color)
{
    color.r = static_cast<f32>(o[(size_t)0].getDouble());
    color.g = static_cast<f32>(o[1].getDouble());
    color.b = static_cast<f32>(o[2].getDouble());
    color.a = static_cast<f32>(o[3].getDouble());
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Color8 & color)
{
    color.r = o[(size_t)0].getInt();
    color.g = o[1].getInt();
    color.b = o[2].getInt();
    color.a = o[3].getInt();
}

//------------------------------------------------------------------------------
inline void serialize(JsonBox::Value & o, const Matrix4 & m)
{
    const f32 * v = m.values();

    JsonBox::Array a;
    a.resize(16);

    for(u32 i = 0; i < 16; ++i)
    {
        a[i] = v[i];
    }

    o = a;
}

//------------------------------------------------------------------------------
inline void unserialize(const JsonBox::Value & o, Matrix4 & m)
{
    SN_ASSERT(o.isArray(), "Expected array to unserialize Matrix4");

    const JsonBox::Array & a = o.getArray();
    f32 v[16];
    for (u32 i = 0; i < 16; ++i)
    {
        v[i] = static_cast<f32>(a[i].getDouble());
    }
    m = Matrix4(v);

    //f32 v[16] = {
    //	a[0].getDouble(), a[1].getDouble(), a[2].getDouble(), a[3].getDouble(),
    //	a[4].getDouble(), a[5].getDouble(), a[6].getDouble(), a[7].getDouble(),
    //	a[8].getDouble(), a[9].getDouble(), a[10].getDouble(), a[11].getDouble(),
    //	a[12].getDouble(), a[13].getDouble(), a[14].getDouble(), a[15].getDouble()
    //};
}

//------------------------------------------------------------------------------
inline void serialize(JsonBox::Value & o, const Quaternion & q)
{
    o[(size_t)0] = q.getW();
    o[1] = q.getX();
    o[2] = q.getY();
    o[3] = q.getZ();
}

//------------------------------------------------------------------------------
inline void unserialize(JsonBox::Value & o, Quaternion & q)
{
    SN_ASSERT(o.isArray(), "Expected array to unserialize Quaternion");

    const JsonBox::Array & a = o.getArray();
    if (a.size() == 4)
    {
        q = Quaternion(
            static_cast<f32>( o[(size_t)0].getDouble() ),
            static_cast<f32>( o[1].getDouble() ),
            static_cast<f32>( o[2].getDouble() ),
            static_cast<f32>( o[3].getDouble() )
        );
    }
    else if (a.size() == 3)
    {
        q.setFromEuler(
            static_cast<f32>( o[(size_t)0].getDouble() ),
            static_cast<f32>( o[1].getDouble() ),
            static_cast<f32>( o[2].getDouble() )
        );
    }
    else
    {
        SN_ERROR("Expected array of size 3 or 4 to unserialize Quaternion");
    }
}

} // namespace sn

#endif // __HEADER_SN_JSON_UTILS__


