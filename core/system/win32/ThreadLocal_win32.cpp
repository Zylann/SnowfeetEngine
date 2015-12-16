#ifndef __HEADER_SN_THREADLOCAL_WIN32__
#define __HEADER_SN_THREADLOCAL_WIN32__

#include "../ThreadLocal.h"
#include <core/util/assert.h>
#include <Windows.h>

namespace sn
{

/// \cond INTERNAL
class ThreadLocalImpl
{
public:
	ThreadLocalImpl() : index(0)
	{
		index = TlsAlloc();
		SN_ASSERT(index != TLS_OUT_OF_INDEXES, "Couldn't allocate thread-local storage (win32)");
	}
	~ThreadLocalImpl()
	{
		TlsFree(index);
	}

	DWORD index;
};
/// \endcond

ThreadLocal::ThreadLocal(void* value /* = nullptr */)
{
	m_impl = new ThreadLocalImpl();
	set(value);
}

ThreadLocal::~ThreadLocal()
{
	delete m_impl;
}

void* ThreadLocal::get() const
{
	return TlsGetValue(m_impl->index);
}

void ThreadLocal::set(void* ptr)
{
	TlsSetValue(m_impl->index, ptr);
}

} // namespace sn

#endif // __HEADER_SN_THREADLOCAL_WIN32__

