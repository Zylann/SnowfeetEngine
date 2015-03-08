/*
RefCounted.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_REFCOUNTED__
#define __HEADER_SN_REFCOUNTED__

#include <core/util/assert.hpp>

namespace sn
{

/// \brief Internal reference counter for objects inheriting RefCounted
class SharedState
{
public:
    /// \brief Constructs a shared state with a reference count of 1
    /// (for both the object and the SharedState itself)
	SharedState() :
		m_refCount(1),
		m_objectRefCount(1)
	{}

	//--------------------------------------------
	// Object reference counting
	//--------------------------------------------

	u32 addObjectRef()
	{
		addRef();
		return ++m_objectRefCount;
	}

	u32 releaseObject()
	{
		u32 orc = --m_objectRefCount;
		release();
		return orc;
	}
	
	u32 getObjectRefCount()
	{
		return m_objectRefCount;
	}

	//--------------------------------------------
	// Reference counting for the counter itself
	// (needed for weakRefs)
	//--------------------------------------------

	u32 addRef()
	{
		return ++m_refCount;
	}

	u32 release()
	{
		if (--m_refCount)
			return m_refCount;
		delete this;
		return 0;
	}

	u32 getRefCount()
	{
		return m_refCount;
	}

private:
	~SharedState()
	{
        SN_ASSERT(m_refCount == 0, "refCount is not zero in destructor");
        SN_ASSERT(m_objectRefCount == 0, "objectRefCount is not zero in destructor");
	}

private:
	u32 m_refCount;
	u32 m_objectRefCount;
};

/// \brief This class must be inherited by objects needing reference counted memory management.
/// If so, such objects must NEVER be destroyed with delete, but by using the release() method.
/// Also, don't use on the stack, but only with dynamic allocation.
/// \warning Not thread safe yet.
class SN_API RefCounted
{
public:

    /// \brief Initializes the reference counted object with 1 reference
    RefCounted()
	{
		m_sharedState = new SharedState();
	}

    /// \brief Call this when you take usage of the object.
    /// \note You don't need to call it if you created the object (if you did, it will leak!).
    /// \return Number or references after the call.
    u32 addRef()
    {
        return m_sharedState->addObjectRef();
    }

    /// \brief Call this when you no longer need the object.
    /// \return Number or references left after the call. If 0, the object is now deleted.
    u32 release()
    {
		u32 rc = m_sharedState->releaseObject();
		if (rc == 0)
            delete this;
        return rc;
    }

    /// \brief Returns the number or usage places this object is currently referenced.
    inline u32 getRefCount() const { return m_sharedState->getObjectRefCount(); }

	/// \brief WARNING: do not access. Reserved for WeakRefs. Here until I find a more "private" solution.
	SharedState * getSharedState() { return m_sharedState; }

protected:
    /// \brief Destructor not exposed because release() must be used instead
    virtual ~RefCounted()
    {
        //SN_ASSERT(getRefCount() == 0, "refCount is not zero in destructor");
    }

private:
	SharedState * m_sharedState;
};

} // namespace sn

#endif // __HEADER_SN_REFCOUNTED__


