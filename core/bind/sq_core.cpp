#include "sq_core.h"

namespace sn
{

void bindCore(HSQUIRRELVM vm)
{
    // Note: the order is important because of class inheritance

    //bindVector3(vm); // not available yet

    bindJoystick(vm);
    
    bindEntity(vm);
    bindEntity3D(vm);
    bindScene(vm);

    bindAsset(vm);

}

} // namespace sn


