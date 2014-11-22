#ifndef __HEADER_SN_REFCOUNTED__
#define __HEADER_SN_REFCOUNTED__

#include <core/util/Log.hpp>

namespace sn
{

class SN_API RefCounted
{
public:

    RefCounted() : m_refCount(1) {}
    virtual ~RefCounted()
    {
        SN_ASSERT(m_refCount == 0, "refCount is not zero in destructor");
    }

    u32 addRef()
    {
        return ++m_refCount;
    }

    u32 release()
    {
        if (--m_refCount == 0)
        {
            delete this;
            return 0;
        }
        return m_refCount;
    }

private:

    u32 m_refCount;
};

} // namespace sn

#endif // __HEADER_SN_REFCOUNTED__


