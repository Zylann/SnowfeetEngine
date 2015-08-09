#ifndef __HEADER_SN_VARIANT_SERIALIZE__
#define __HEADER_SN_VARIANT_SERIALIZE__

#include <core/util/Variant.h>
#include <core/math/Range.h>
#include <core/math/Color.h>
#include <core/math/Area.h>
#include <vector>
#include <unordered_set>

namespace sn
{

//------------------------------------------------------------------------------
// Get any JSON number into a float, zero if not a number
inline f32 unserializeAsFloat(const Variant & v)
{
	switch (v.getType().id)
	{
	case SN_VT_INT: return static_cast<f32>(v.getInt());
	case SN_VT_FLOAT: return static_cast<f32>(v.getFloat());
	default: return 0;
	}
}

//------------------------------------------------------------------------------
inline void serialize(Variant & o, f32 v) { o = v; }
inline void serialize(Variant & o, s32 v) { o = v; }
inline void serialize(Variant & o, u32 v) { o = (s32)v; }
inline void serialize(Variant & o, u8 v) { o = (s32)v; }
inline void serialize(Variant & o, bool v) { o = v; }
inline void serialize(Variant & o, const std::string & s) { o = s; }

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, f32 & v) { v = unserializeAsFloat(o); }

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, s32 & v)
{
    if (o.isInt())
        v = o.getInt();
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, u32 & v)
{
    if (o.isInt())
        v = static_cast<u32>(o.getInt());
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, u8 & v)
{
    if (o.isInt())
        v = static_cast<u32>(o.getInt());
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, bool & v)
{
    if (o.isBool())
        v = static_cast<u32>(o.getBool());
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, std::string & s)
{
    if (o.isString())
        s = o.getString();
}

//------------------------------------------------------------------------------
template <typename T>
inline void serialize(Variant & o, const Range<T> & range)
{
    o.setArray();
	o[(size_t)0] = range.min();
	o[1] = range.max();
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Range<f32> & range)
{
    if (o.isArray())
    {
	    range.set(
		    sn::unserializeAsFloat(o[(size_t)0]),
		    sn::unserializeAsFloat(o[1])
	    );
    }
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Range<s32> & range)
{
    if (o.isArray())
    	range.set(o[(size_t)0].getInt(), o[1].getInt());
}

//------------------------------------------------------------------------------
template <typename List_T>
void serialize(Variant & o, const List_T & v)
{
    Variant::Array a;
	a.resize(v.size());
	u32 i = 0;
	for (auto it = v.cbegin(); it != v.cend(); ++it)
	{
		a[i] = *it;
		++it;
		++i;
	}
	o.setArray(a);
}

//------------------------------------------------------------------------------
template <typename T>
void unserialize(const Variant & o, std::vector<T> & v)
{
    if (o.isArray())
    {
	    const Variant::Array & a = o.getArray();
	    if (v.size() < a.size())
		    v.resize(a.size());
	    for (u32 i = 0; i < a.size(); ++i)
	    {
		    const Variant & av = a[i];
		    unserialize(av, v[i]);
	    }
    }
}

//------------------------------------------------------------------------------
template <typename T>
void unserialize(const Variant & o, std::unordered_set<T> & v)
{
    if (o.isArray())
    {
	    const Variant::Array & a = o.getArray();
        v.reserve(a.size());
	    for (u32 i = 0; i < a.size(); ++i)
	    {
		    T elem;
		    unserialize(a[i], elem);
		    v.insert(elem);
	    }
    }
}

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
inline void serialize(Variant & o, const Vector<T, N> & vec)
{
    o.setArray();
	for (size_t i = 0; i < N; ++i)
		o[i] = vec[i];
}

//------------------------------------------------------------------------------
template <unsigned int N>
inline void unserialize(const Variant & o, Vector<f32, N> & vec)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
        if (a.size() == N)
        {
	        for (size_t i = 0; i < N; ++i)
		        unserialize(a[i], vec[i]);
        }
        else
        {
            u32 count = std::min(a.size(), N);
	        for (size_t i = 0; i < count; ++i)
		        unserialize(a[i], vec[i]);
        }
    }
}

//------------------------------------------------------------------------------
template <unsigned int N>
inline void unserialize(const Variant & o, Vector<s32, N> & vec)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
        if (a.size() == N)
        {
	        for (size_t i = 0; i < N; ++i)
		        unserialize(a[i].getInt(), vec[i]);
        }
        else
        {
            u32 count = std::min(a.size(), N);
	        for (size_t i = 0; i < count; ++i)
		        unserialize(a[i].getInt(), vec[i]);
        }
    }
}

//------------------------------------------------------------------------------
inline u32 unserializeFlooredU32(const Variant o)
{
	s32 n = o.getInt();
    if (n < 0)
        return 0;
    return static_cast<u32>(n);
}

//------------------------------------------------------------------------------
template <unsigned int N>
inline void unserialize(const JsonBox::Value & o, Vector<u32, N> & vec)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
        if (a.size() == N)
        {
	        for (size_t i = 0; i < N; ++i)
		        vec[i] = unserializeFlooredU32(a[i]);
        }
        else
        {
            u32 count = std::min(a.size(), N);
	        for (size_t i = 0; i < count; ++i)
		        vec[i] = unserializeFlooredU32(a[i]);
        }
    }
}

//------------------------------------------------------------------------------
template <typename T>
inline void serialize(Variant & o, const Area<T, 2> & rect)
{
    o.setDictionary();
	o["x"] = rect.x();
	o["y"] = rect.y();
	o["w"] = rect.width();
	o["h"] = rect.height();
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Area<f32, 2> & rect)
{
    if (o.isDictionary())
    {
	    rect.x() = static_cast<f32>(o["x"].getFloat());
	    rect.y() = static_cast<f32>(o["y"].getFloat());
	    rect.width() = static_cast<f32>(o["w"].getFloat());
	    rect.height() = static_cast<f32>(o["h"].getFloat());
    }
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Area<s32, 2> & rect)
{
    if (o.isDictionary())
    {
	    rect.x() = o["x"].getInt();
	    rect.y() = o["y"].getInt();
	    rect.width() = o["w"].getInt();
	    rect.height() = o["h"].getInt();
    }
}

//------------------------------------------------------------------------------
inline void serialize(Variant & o, const Color & color)
{
    o.setArray();
	o[(size_t)0] = color.r;
	o[1] = color.g;
	o[2] = color.b;
	o[3] = color.a;
}

//------------------------------------------------------------------------------
inline void serialize(Variant & o, const Color8 & color)
{
    o.setArray();
	o[(size_t)0] = color.r;
	o[1] = color.g;
	o[2] = color.b;
	o[3] = color.a;
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Color & color)
{
    if (o.isArray())
    {
	    unserialize(o[(size_t)0], color.r);
	    unserialize(o[1], color.g);
	    unserialize(o[2], color.b);
	    if (o.getArray().size() >= 4)
		    unserialize(o[3], color.a);
    }
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Color8 & color)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
	    color.r = a[0].getInt();
	    color.g = a[1].getInt();
	    color.b = a[2].getInt();
        if (a.size() >= 4)
    	    color.a = a[3].getInt();
    }
}

//------------------------------------------------------------------------------
inline void serialize(Variant & o, const Matrix4 & m)
{
	const f32 * v = m.values();

	Variant::Array a;
	a.resize(16);

	for (u32 i = 0; i < 16; ++i)
	{
		a[i] = v[i];
	}

	o.setArray(a);
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Matrix4 & m)
{
	SN_ASSERT(o.isArray(), "Expected array to unserialize Matrix4");

	const Variant::Array & a = o.getArray();
	f32 v[16];
	for (u32 i = 0; i < 16; ++i)
	{
		v[i] = static_cast<f32>(a[i].getFloat());
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
inline void serialize(Variant & o, const Quaternion & q)
{
    o.setArray();
	o[(size_t)0] = q.getW();
	o[1] = q.getX();
	o[2] = q.getY();
	o[3] = q.getZ();
}

//------------------------------------------------------------------------------
inline void unserialize(const Variant & o, Quaternion & q)
{
	SN_ASSERT(o.isArray(), "Expected array to unserialize Quaternion");

	const Variant::Array & a = o.getArray();
	if (a.size() == 4)
	{
		q = Quaternion(
			static_cast<f32>(o[(size_t)0].getFloat()),
			static_cast<f32>(o[1].getFloat()),
			static_cast<f32>(o[2].getFloat()),
			static_cast<f32>(o[3].getFloat())
	    );
	}
	else if (a.size() == 3)
	{
		Vector3f euler;
		sn::unserialize(o, euler);
		q.setFromEuler(euler.x(), euler.y(), euler.z());
	}
	else
	{
		SN_ERROR("Expected array of size 3 or 4 to unserialize Quaternion");
	}
}

} // namespace sn

#endif // __HEADER_SN_VARIANT_SERIALIZE__

