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
		m_ptr(nullptr),
        m_sharedState(nullptr)
	{}

	WeakRef(RefCounted_T * ptr) :
		m_ptr(ptr)
	{
		m_sharedState = ptr->getSharedState();
		m_sharedState->addRef();
	}

	~WeakRef()
	{
        if (m_sharedState)
    		m_sharedState->release();
	}

	//---------------------------------------------
	// Methods
	//---------------------------------------------

	void set(RefCounted_T * ptr)
	{
        // Note: we need to addRef before releasing.
        // (what if m_ptr == ptr and refCount == 1?)
        if (ptr)
            ptr->getSharedState()->addRef();

        // Release old reference
        if (m_ptr)
			m_sharedState->release();

        // Assign new pointer
		m_ptr = ptr;
        if (m_ptr)
            m_sharedState = ptr->getSharedState();
    }

	bool isNull()
	{
		return m_ptr == nullptr || m_sharedState->getObjectRefCount() == 0;
	}

	RefCounted_T * get() const
	{
		return m_ptr;
	}

	//---------------------------------------------
	// Operators
	//---------------------------------------------

    inline WeakRef & operator=(WeakRef & other)
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

