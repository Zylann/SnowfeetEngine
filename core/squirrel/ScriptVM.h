#ifndef __HEADER_SQUIRREL_VM__
#define __HEADER_SQUIRREL_VM__

#include <core/squirrel/Script.h>
#include <core/types.hpp>

namespace squirrel
{
    
/// \brief C++ wrapper for a Squirrel VM
class SN_API VM
{
public:
    VM(SQInteger initialStackSize = 1024);
    ~VM();

    HSQUIRRELVM getSquirrelVM() const { return m_vm; }

private:
    static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);

private:
    HSQUIRRELVM m_vm;

};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_VM__

