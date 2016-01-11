#ifndef __HEADER_SN_LOCK__
#define __HEADER_SN_LOCK__

#include <core/system/Mutex.h>

namespace sn
{

class SN_API Lock
{
public:

    explicit Lock(Mutex & mutex): r_mutex(mutex)
    {
        r_mutex.lock();
    }

    ~Lock()
    {
        r_mutex.unlock();
    }

private:

    Mutex & r_mutex;
};

} // namespace sn

#endif // __HEADER_SN_LOCK__

