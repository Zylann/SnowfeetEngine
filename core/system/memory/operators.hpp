#ifndef __HEADER_SN_MEMORY_OPERATORS__
#define __HEADER_SN_MEMORY_OPERATORS__

#include <core/system/memory/MemoryManager.hpp>
#include <core/types.hpp>

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

// TODO FIXME 'delete' calls triggered by 'new' exceptions must be handled!!

template <typename T>
void _singleDelete(T * ptr, const char * file, u32 line)
{
	if (ptr) // The standard states a delete on null is safe
	{
		// Call destructor
		ptr->~T();

		// Free memory
		sn::MemoryManager::get().free(ptr, SN_MAT_SINGLE, file, line);
	}
}

template <typename T>
T * _arrayNew(T * ptr, u32 count, const char * file, u32 line)
{
	// Prepend the size of the array (extra memory should have been allocated before)
	((size_t*)ptr)[0] = count;

	// Move the pointer forward
	ptr = (T*)((size_t*)ptr[1]);

	// Call constructors
	for (u32 i = 0; i < count; ++i)
		new(&ptr[i]) T();

	return ptr;
}

template <typename T>
void _arrayDelete(T * ptr, const char * file, u32 line)
{
	if (ptr)
	{
		// Get size of the array
		u32 count = ((size_t*)ptr)[-1];

		// Call destructors
		for (u32 i = 0; i < count; ++i)
			ptr[i].~T();

		// Free memory
		sn::MemoryManager::get().free(&( (size_t*)ptr[-1] ), file, line);
	}
}

} // namespace sn

#define SN_NEW(_type) \
    new(sn::MemoryManager::get().allocate(sizeof(_type), sn::SN_MAT_SINGLE, __FILE__, __LINE__)) _type

#define SN_NEW_ARRAY(_type, _count) \
    sn::_arrayNew(sn::MemoryManager::get().allocate(sizeof(_type)*_count + sizeof(size_t), sn::SN_MAT_ARRAY, __FILE__, __LINE__), _count)

#define SN_DELETE(_ptr) \
	_singleDelete(_ptr, __FILE__, __LINE__);

#define SN_DELETE_ARRAY(_ptr) \
	_arrayDelete(_ptr, __FILE__, __LINE__);

#endif // __HEADER_SN_MEMORY_OPERATORS__

