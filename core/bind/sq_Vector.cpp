#include "sq_core.h"
#include "../squirrel/Class.h"
#include "../squirrel/Table.h"

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

    squirrel::Class c(vm, className);

    // Bind constructor
    c.setConstructor(constructor);

    // Make it accessible from the root table
    squirrel::RootTable(vm).setObject(className, c);
}

} // namespace sn


