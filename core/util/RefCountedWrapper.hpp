/*
RefCountedWrapper.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_REFCOUNTEDWRAPPER__
#define __HEADER_SN_REFCOUNTEDWRAPPER__

#include <core/util/RefCounted.hpp>

namespace sn
{

/// \brief Wraps an object inheriting RefCounted and 
/// automatically addRef() and release() it upon construction and destruction.
template <class RefCounted_T>
class RefCountedWrapper
{
public:
	RefCountedWrapper() :
		m_obj(nullptr)
	{}

    explicit RefCountedWrapper(RefCounted_T * obj):
        m_obj(obj)
    {
		addRef();
    }

    RefCountedWrapper(const RefCountedWrapper & other):
		m_obj(other.m_obj)
    {
		addRef();
	}

    ~RefCountedWrapper()
    {
		release();
	}

	inline bool isNull() const { return m_obj == nullptr; }

    inline RefCounted_T * operator->() { return m_obj; }
    inline RefCounted_T & operator*() { return *m_obj; }

    inline const RefCounted_T * operator->() const { return m_obj; }
    inline const RefCounted_T & operator*() const { return *m_obj; }

	inline RefCountedWrapper & operator=(RefCountedWrapper & other)
	{
		release();
		m_obj = other.m_obj;
		addRef();
		return *this;
	}

private:
	inline void addRef()
	{
		if (m_obj)
			m_obj->addRef();
	}

	inline void release()
	{
		if (m_obj)
			m_obj->release();
		m_obj = nullptr;
	}

    RefCounted_T * m_obj;
};

} // namespace sn

#endif // __HEADER_SN_REFCOUNTEDWRAPPER__
