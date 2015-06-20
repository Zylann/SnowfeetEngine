#include "sq_core.h"
#include "../squirrel/ScriptClass.h"
#include "../squirrel/ScriptTable.h"

namespace sn
{
    namespace
    {
        SQInteger constructor(HSQUIRRELVM vm)
        {
            return 0;
        }

        // TODO
    }

void bindVector3(HSQUIRRELVM vm)
{
    const char * className = "Vector3";

    ScriptClass c(vm, className);

    // Bind constructor
    c.setConstructor(constructor);

    // Make it accessible from the root table
    ScriptRootTable(vm).setObject(className, c);
}

} // namespace sn


