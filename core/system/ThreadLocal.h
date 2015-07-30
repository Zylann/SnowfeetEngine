#ifndef __HEADER_SN_THREADLOCAL__
#define __HEADER_SN_THREADLOCAL__

#include <core/export.h>

namespace sn
{

class ThreadLocalImpl;

/// \brief Storage for variables that are local to a thread.
/// For instance, if two threads access the same ThreadLocal,
/// they can get/set a different value because the system returns a different store for them.
/// This is useful for non-thread-safe "global" variables that are local to threads.
class SN_API ThreadLocal
{
public:
	ThreadLocal(void* value = nullptr);
	~ThreadLocal();

	void* get() const;
	void set(void* value);

private:
	ThreadLocalImpl * m_impl;
};

} // namespace sn

#endif // __HEADER_SN_THREADLOCAL__

