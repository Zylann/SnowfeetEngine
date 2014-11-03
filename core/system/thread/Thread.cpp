#include "Thread.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Thread::Thread(void(*func)()):
    r_func(func)
{
}

//------------------------------------------------------------------------------
void Thread::run()
{
    (*r_func)();
}

} // namespace sn

