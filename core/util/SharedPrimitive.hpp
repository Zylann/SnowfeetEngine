#ifndef __HEADER_SN_SHAREDPRIMITIVE__
#define __HEADER_SN_SHAREDPRIMITIVE__

#include <core/types.hpp>

namespace sn
{

//template <typename T>
//class SharedPrimitive
//{
//public:
//    SharedPrimitive(const T value):
//        m_refCount(0),
//        m_value(value)
//    {}
//
//    u32 addRef()
//    {
//        return ++m_refCount;
//    }
//
//    u32 release()
//    {
//        if (--m_refCount)
//        {
//            return m_refCount;
//        }
//        else
//        {
//            delete this;
//            return 0;
//        }
//    }
//
//    T get() { return m_value; 
//    void set(T value) { m_value = value; }
//
//private:
//    ~SharedPrimitive() {}
//
//private:
//    u32 m_refCount;
//    T m_value;
//};

//typedef SharedPrimitive<bool> SharedBool;

} // namespace sn

#endif // __HEADER_SN_SHAREDPRIMITIVE__

