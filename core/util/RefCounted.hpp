#ifndef __HEADER_SN_REFCOUNTED__
#define __HEADER_SN_REFCOUNTED__

#include <core/util/Log.hpp>

namespace sn
{

/// \brief This class must be inherited by objects needing reference counted memory management.
/// If so, such objects must NEVER be destroyed with delete, but by using the release() method.
/// \warning Not thread safe yet.
class SN_API RefCounted
{
public:

    /// \brief Initializes the reference counted object with 1 reference
    RefCounted() : m_refCount(1) {}

    /// \brief Call this when you take usage of the object.
    /// \note You don't need to call it if you created the object (if you did, it will leak!).
    /// \return Number or references after the call.
    u32 addRef()
    {
        return ++m_refCount;
    }

    /// \brief Call this when you no longer need the object.
    /// \return Number or references left after the call. If 0, the object is now deleted.
    u32 release()
    {
        if (--m_refCount == 0)
        {
            delete this;
            return 0;
        }
        return m_refCount;
    }

    /// \brief Returns the number or usage places this object is currently referenced.
    inline u32 getRefCount() const { return m_refCount; }

protected:

    /// \brief Destructor not exposed because release() must be used instead
    virtual ~RefCounted()
    {
        SN_ASSERT(m_refCount == 0, "refCount is not zero in destructor");
    }

private:

    // TODO Make it thread safe?
    u32 m_refCount;
};

} // namespace sn

#endif // __HEADER_SN_REFCOUNTED__


