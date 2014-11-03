#ifndef __HEADER_SN_REFCOUNTED__
#define __HEADER_SN_REFCOUNTED__

#include <core/types.hpp>

namespace sn
{

class SN_API RefCounted
{
public:
    RefCounted() : m_refCount(1) {}
    virtual ~RefCounted() {}
    void addRef()
    {
        ++m_refCount;
    }
    void release()
    {
        --m_refCount;
        if (m_refCount == 0)
            delete this;
    }
private:
    u32 m_refCount;
};

} // namespace sn

#endif // __HEADER_SN_REFCOUNTED__


