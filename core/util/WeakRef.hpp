#ifndef __HEADER_SN_WEAKREF__
#define __HEADER_SN_WEAKREF__

#include <core/util/RefCounted.hpp>

namespace sn
{

/// \brief RAII container without ownership for RefCounted types.
/// isNull() will return correct results if the pointed object has been destroyed.
template <class RefCounted_T>
class WeakRef
{
public:
	WeakRef() :
		m_ptr(nullptr)
	{}

	WeakRef(RefCounted_T * ptr) :
		m_ptr(ptr)
	{
		m_sharedState = ptr->getSharedState();
		m_sharedState->addRef();
	}

	~WeakRef()
	{
		m_sharedState->release();
	}

	//---------------------------------------------
	// Methods
	//---------------------------------------------

	void set(RefCounted_T * ptr)
	{
		if (m_ptr)
			m_sharedState->release();
		m_ptr = ptr;
		if (m_ptr)
		{
			m_sharedState = m_ptr->getSharedState();
			m_sharedState->addRef();
		}
	}

	bool isNull()
	{
		return m_ptr == nullptr || m_sharedState->getObjectRefCount() == 0;
	}

	RefCounted_T * get()
	{
		return m_ptr;
	}

	//---------------------------------------------
	// Operators
	//---------------------------------------------

	inline WeakRef & operator=(SharedRef & other)
	{
		set(other.m_ptr);
		return *this;
	}

private:
	RefCounted_T * m_ptr;
	SharedState * m_sharedState;
};

} // namespace sn

#endif // __HEADER_SN_WEAKREF__
