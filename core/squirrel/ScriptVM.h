#ifndef __HEADER_SN_SCRIPTVM__
#define __HEADER_SN_SCRIPTVM__

#include <core/squirrel/Script.h>
#include <core/types.hpp>

namespace sn
{
    
/// \brief C++ wrapper for a Squirrel VM
class SN_API ScriptVM
{
public:
    ScriptVM(u32 initialStackSize = 1024);
    ~ScriptVM();

    HSQUIRRELVM getSquirrelVM() const { return m_vm; }

private:
    static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);

private:
    HSQUIRRELVM m_vm;

};

} // namespace sn

#endif // __HEADER_SN_SCRIPTVM__

