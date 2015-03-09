#ifndef __HEADER_SN_MEMORY_OPERATORS__
#define __HEADER_SN_MEMORY_OPERATORS__

#include <core/system/memory/MemoryManager.hpp>

//inline void* operator new(size_t size, const char* file, int line)
//{
//    return sn::MemoryManager::get().allocate(size, file, line, false);
//}
//
//inline void* operator new[](size_t size, const char* file, int line)
//{
//    return sn::MemoryManager::get().allocate(size, file, line, true);
//}
//
//inline void operator delete(void* ptr)
//{
//    //::free(ptr);
//    sn::MemoryManager::get().free(ptr, false);
//}
//
//inline void operator delete[](void* ptr)
//{
//    //::free(ptr);
//    sn::MemoryManager::get().free(ptr, true);
//}
//
//inline void operator delete(void* ptr, const char* file, int line)
//{
//    sn::MemoryManager::get().nextDelete(file, line);
//    sn::MemoryManager::get().free(ptr, false);
//}
//
//inline void operator delete[](void* ptr, const char* file, int line)
//{
//    sn::MemoryManager::get().nextDelete(file, line);
//    sn::MemoryManager::get().free(ptr, true);
//}

//#if defined(SN_BUILD_DEBUG) && !defined(new)
//    #define new new(__FILE__, __LINE__)
//    #define delete sn::MemoryManager::get().nextDelete(__FILE__, __LINE__), delete
//#endif

namespace sn
{

}
// Draft

//#define SN_NEW(_type, ...) \
//    new(sn::MemoryManager::get().allocate(sizeof(_type), __FILE__, __LINE__, sn::SN_MAT_SINGLE)) _type(##__VA_ARGS__)
//
//#define SN_NEW_ARRAY(_type, _count) \
//    new(sn::MemoryManager::get().allocate(sizeof(_type), __FILE__, __LINE__, sn::SN_MAT_ARRAY)) _type[_count]
//
//#define SN_DELETE(_ptr) \
//    delete(sn::MemoryManager::get().free( _ptr
//
//#define SN_DELETE_ARRAY(_ptr) \
//    delete[] _ptr

#endif // __HEADER_SN_MEMORY_OPERATORS__

