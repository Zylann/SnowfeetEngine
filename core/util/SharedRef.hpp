/*
RefCountedWrapper.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SHAREDREF__
#define __HEADER_SN_SHAREDREF__

#include <core/util/RefCounted.hpp>

namespace sn
{

/// \brief RAII ownership container for RefCounted types.
/// Automatically calls addRef() and release().
template <class RefCounted_T>
class SN_API SharedRef
{
public:
    SharedRef() :
		m_ptr(nullptr)
	{}

    explicit SharedRef(RefCounted_T * ptr) :
        m_ptr(ptr)
    {
		if (m_ptr)
			m_ptr->addRef();
    }

    ~SharedRef()
    {
		if (m_ptr)
			m_ptr->release();
	}
	
	//-------------------------------------------
	// Methods
	//-------------------------------------------

	inline bool isNull() const { return m_ptr == nullptr; }
	inline RefCounted_T * get() const { return m_ptr; }

	inline void set(RefCounted_T * ptr)
	{
        // Note: we need to addRef before releasing.
        // (what if m_ptr == ptr and refCount == 1?)
        if (ptr)
            ptr->addRef();
        if (m_ptr)
			m_ptr->release();
		m_ptr = ptr;
	}

	//-------------------------------------------
	// Operators
	//-------------------------------------------

	//inline RefCounted_T * operator->() { return m_obj; }
    //inline RefCounted_T & operator*() { return *m_obj; }

    //inline const RefCounted_T * operator->() const { return m_obj; }
    //inline const RefCounted_T & operator*() const { return *m_obj; }

	inline SharedRef & operator=(SharedRef & other)
	{
		set(other.m_ptr);
		return *this;
	}

private:
    RefCounted_T * m_ptr;

};

} // namespace sn

#endif // __HEADER_SN_SHAREDREF__
