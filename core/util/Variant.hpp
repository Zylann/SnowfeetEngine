/*
Variant.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VARIANT__
#define __HEADER_SN_VARIANT__

namespace sn
{

// See http://www.ojdip.net/2013/10/implementing-a-variant-type-in-cpp/

// TODO Don't use typeid, but type traits, because the intented use needs stable IDs.
// TODO Variant typedef containing engine's primitives

//------------------------------------------------------------------------------
template <typename T, typename...Ts>
struct TVariantHelper
{
	static const size_t maxSize =
	sizeof(T) > TVariantHelper<Ts...>::maxSize ?
	sizeof(T) : TVariantHelper<Ts...>::maxSize;

	inline static void destroy(size_t typeID, void * data)
	{
		if (typeID == TVariantHelper<T>::getTypeID())
			reinterpret_cast<T*>(data)->~F();
		else
			TVariantHelper::<Ts...>::destroy(typeID, data);
	}

	inline static void copy(size_t oldType, const void * oldData, const void * newData)
	{
		if (oldType == TVariantHelper<T>::getTypeID())
			new (newData)T(*reinterpret_cast<const T*>(oldData));
		else
			TVariantHelper<Ts...>::copy(oldType, oldData, newData);
	}
};

//------------------------------------------------------------------------------
template <typename T>
struct TVariantHelper<T>
{
	static const size_t maxSize = sizeof(T);

	inline static void destroy(size_t typeID, void * data)
	{
		if (typeID == getTypeID<F>())
			reinterpret_cast<T*>(data)->~F();
	}

	inline static void copy(size_t oldType, const void * oldData, const void * newData)
	{
		if (oldType == TVariantHelper<T>::getTypeID())
			new (newData)T(*reinterpret_cast<const T*>(oldData));
	}

	inline static size_t getTypeID()
	{
		return typeid(T).hash_code();
	}
};

//------------------------------------------------------------------------------
template <typename...Ts>
class TVariant
{
public:
	TVariant() :
		m_typeID(invalidType())
	{}

	// Move constructor
	TVariant(TVariant<Ts...> && old):
		m_typeID(old.m_typeID),
		m_data(old.m_data)
	{
		old.m_typeID = invalidType();
	}

	// Copy constructor
	TVariant(const TVariant<Ts...> & old) :
		m_typeID(old.m_typeID)
	{
		// Deep copy
		Helper::copy(old.m_typeID, &old.m_data, &m_data);
	}

	~TVariant()
	{
		Helper::destroy(m_typeID, &m_data);
	}

	template <typename T, typename...Args_T>
	void set(Args_T&&... args)
	{
		Helper::destroy(m_typeID, &data);
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
	char m_data[Helper::maxSize];
};

} // namespace sn

#endif // __HEADER_SN_VARIANT__

