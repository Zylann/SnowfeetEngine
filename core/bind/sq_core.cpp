#include "sq_core.h"

namespace sn
{

void bindCore(HSQUIRRELVM vm)
{
    // Note: the order is important because of class inheritance

    //bindVector3(vm); // not available yet
    
    bindEntity(vm);
    bindEntity3D(vm);
    bindScene(vm);
    bindDrawable(vm);

}

} // namespace sn


