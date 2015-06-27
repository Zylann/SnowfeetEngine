#include "sq_Quaternion.h"
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

void bindQuaternion(HSQUIRRELVM vm)
{
    const char * className = "Quaternion";

    squirrel::Class c(vm, className);

    // Bind constructor
    c.setConstructor(constructor);

    // Make it accessible from the root table
    squirrel::RootTable(vm).setObject(className, c);
}

} // namespace sn

