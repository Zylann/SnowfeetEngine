/*
Variant.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VARIANT__
#define __HEADER_SN_VARIANT__

#include <core/types.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Vector3.hpp>
#include <core/math/Quaternion.hpp>
#include <core/math/Matrix4.hpp>

namespace sn
{

// See http://www.ojdip.net/2013/10/implementing-a-variant-type-in-cpp/

// TODO Don't use typeid, but type traits, because the intented use needs stable IDs.
// TODO Variant typedef containing engine's primitives

//------------------------------------------------------------------------------
//template <typename T, typename...Ts>
//struct TVariantHelper
//{
//	static const size_t maxSize =
//	sizeof(T) > TVariantHelper<Ts...>::maxSize ?
//	sizeof(T) : TVariantHelper<Ts...>::maxSize;
//
//	inline static void destroy(size_t typeID, void * data)
//	{
//		if (typeID == TVariantHelper<T>::getTypeID())
//			reinterpret_cast<T*>(data)->~F();
//		else
//			TVariantHelper::<Ts...>::destroy(typeID, data);
//	}
//
//	inline static void copy(size_t oldType, const void * oldData, const void * newData)
//	{
//		if (oldType == TVariantHelper<T>::getTypeID())
//			new (newData)T(*reinterpret_cast<const T*>(oldData));
//		else
//			TVariantHelper<Ts...>::copy(oldType, oldData, newData);
//	}
//};
//
////------------------------------------------------------------------------------
//template <typename T>
//struct TVariantHelper<T>
//{
//	static const size_t maxSize = sizeof(T);
//
//	inline static void destroy(size_t typeID, void * data)
//	{
//		if (typeID == getTypeID<F>())
//			reinterpret_cast<T*>(data)->~F();
//	}
//
//	inline static void copy(size_t oldType, const void * oldData, const void * newData)
//	{
//		if (oldType == TVariantHelper<T>::getTypeID())
//			new (newData)T(*reinterpret_cast<const T*>(oldData));
//	}
//
//	inline static size_t getTypeID()
//	{
//		return typeid(T).hash_code();
//	}
//};
//
////------------------------------------------------------------------------------
//template <typename...Ts>
//class TVariant
//{
//public:
//	TVariant() :
//		m_typeID(invalidType())
//	{}
//
//	// Move constructor
//	TVariant(TVariant<Ts...> && old):
//		m_typeID(old.m_typeID),
//		m_data(old.m_data)
//	{
//		old.m_typeID = invalidType();
//	}
//
//	// Copy constructor
//	TVariant(const TVariant<Ts...> & old) :
//		m_typeID(old.m_typeID)
//	{
//		// Deep copy
//		Helper::copy(old.m_typeID, &old.m_data, &m_data);
//	}
//
//	~TVariant()
//	{
//		Helper::destroy(m_typeID, &m_data);
//	}
//
//	template <typename T, typename...Args_T>
//	void set(Args_T&&... args)
//	{
//		Helper::destroy(m_typeID, &data);
//		new (&m_data) T(std::forward<Args_T>(args)...);
//		m_typeID = Helper<T>::getTypeID();
//	}
//
//	template <typename T>
//	T & get()
//	{
//		if (m_typeID == TVariantHelper<T>::getTypeID())
//			return *reinterpret_cast<T*>(&m_data);
//		else
//			throw std::bad_cast();
//	}
//
//	// Move assign
//	TVariant<Ts...> & operator=(TVariant<Ts...> && old)
//	{
//		m_data = old.m_data;
//		old.m_typeID = invalidType();
//		return *this;
//	}
//
//	TVariant<Ts...> & operator=(TVariant<Ts...> old)
//	{
//		std::swap(m_data, old.m_data);
//		std::swap(m_typeID, old.m_typeID);
//		return *this;
//	}
//
//	template<typename T>
//	void is()
//	{
//		return m_typeID == TVariantHelper<T>::getTypeID();
//	}
//
//	void valid()
//	{
//		return m_typeID != invalid_type();
//	}
//
//	static inline size_t invalidType()
//	{
//		return TVariantHelper<void>::getTypeID();
//	}
//
//private:
//	typedef TVariantHelper<Ts...> Helper;
//
//	size_t m_typeID;
//	char m_data[Helper::maxSize];
//};

//------------------------------------------------------------------------------
template <typename T> 
struct VariantTypeTraits {};

template <> struct 
VariantTypeTraits<bool> 
{
    static const size_t ID = 1;
    static const bool defaultValue = false;
};

template <>
struct VariantTypeTraits<s32>
{
    static const size_t ID = 2;
    static const s32 defaultValue = false;
};

template <>
struct VariantTypeTraits<f32>
{
    static const size_t ID = 3;
    static constexpr f32 defaultValue = 0;
};

template <>
struct VariantTypeTraits<std::string>
{
    static const size_t ID = 4;
    static const std::string defaultValue;
};

template <>
struct VariantTypeTraits<Vector2i>
{
    static const size_t ID = 5;
    static const Vector2i defaultValue;
};

template <>
struct VariantTypeTraits<Vector2f>
{
    static const size_t ID = 6;
    static const Vector2f defaultValue;
};

template <>
struct VariantTypeTraits<Vector3i>
{
    static const size_t ID = 7;
    static const Vector3i defaultValue;
};

template <>
struct VariantTypeTraits<Vector3f>
{ 
    static const size_t ID = 8;
    static const Vector3f defaultValue;
};

template <>
struct VariantTypeTraits<Vector<s32,4>>
{
    static const size_t ID = 9;
    static const Vector<s32,4> defaultValue;
};

template <>
struct VariantTypeTraits<Vector<f32,4>>
{
    static const size_t ID = 10;
    static const Vector<f32, 4> defaultValue;
};

template <>
struct VariantTypeTraits<Quaternion>
{
    static const size_t ID = 11;
    static const Quaternion defaultValue;
};

template <>
struct VariantTypeTraits<Matrix4>
{
    static const size_t ID = 12;
    static const Matrix4 defaultValue;
};

//------------------------------------------------------------------------------
template <typename T, typename...Ts>
struct TVariantHelper
{
	inline static void destroy(size_t typeID, void * ptr)
	{
		if (typeID == TVariantHelper<T>::ID)
			delete reinterpret_cast<T*>(ptr);
		else
			TVariantHelper::<Ts...>::destroy(typeID, ptr);
	}

	inline static void copy(size_t oldType, const void * oldPtr, const void *& newPtr)
	{
		if (oldType == TVariantHelper<T>::ID)
			newPtr = new T(*reinterpret_cast<const T*>(oldPtr));
		else
			TVariantHelper<Ts...>::copy(oldType, oldPtr, newPtr);
	}
};

//------------------------------------------------------------------------------
template <typename T>
struct TVariantHelper<T>
{
	inline static void destroy(size_t typeID, void * ptr)
	{
		if (typeID == VariantTypeTraits<F>::ID)
            delete reinterpret_cast<T*>(ptr);
	}

	inline static void copy(size_t oldType, const void * oldPtr, const void *& newPtr)
	{
		if (oldType == VariantTypeTraits<T>::ID)
			newPtr = new T(*reinterpret_cast<const T*>(oldPtr));
	}
};

//------------------------------------------------------------------------------
template <typename...Ts>
class TVariant
{
public:
	TVariant() :
		m_typeID(invalidType()),
        m_ptr(nullptr)
	{}

	// Copy constructor
	TVariant(const TVariant<Ts...> & old) :
		m_typeID(old.m_typeID)
	{
		// Deep copy
		Helper::copy(old.m_typeID, old.m_ptr, m_ptr);
	}

	~TVariant()
	{
		Helper::destroy(m_typeID, &m_ptr);
	}

	template <typename T, typename...Args_T>
	void set(Args_T&&... args)
	{
        Helper::destroy(m_typeID, &m_ptr);
		new (&m_data) T(std::forward<Args_T>(args)...);
		m_typeID = Helper<T>::getTypeID();
	}

	template <typename T>
	T & get()
	{
		if (m_typeID == TVariantHelper<T>::getTypeID())
			return *reinterpret_cast<T*>(&m_data);
		else
			throw std::bad_cast();
	}

	// Move assign
	TVariant<Ts...> & operator=(TVariant<Ts...> && old)
	{
		m_data = old.m_data;
		old.m_typeID = invalidType();
		return *this;
	}

	TVariant<Ts...> & operator=(TVariant<Ts...> old)
	{
		std::swap(m_data, old.m_data);
		std::swap(m_typeID, old.m_typeID);
		return *this;
	}

	template<typename T>
	void is()
	{
		return m_typeID == TVariantHelper<T>::getTypeID();
	}

	void valid()
	{
		return m_typeID != invalid_type();
	}

	static inline size_t invalidType()
	{
		return TVariantHelper<void>::getTypeID();
	}

private:
	typedef TVariantHelper<Ts...> Helper;

	size_t m_typeID;
	void * m_ptr;
};

} // namespace sn

#endif // __HEADER_SN_VARIANT__

