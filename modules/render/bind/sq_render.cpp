#include "sq_render.h"

namespace sn
{

void bindRenderModule(HSQUIRRELVM vm)
{
    bindDrawable(vm);
    bindMeshEntity(vm);
    bindCamera(vm);
    bindMaterial(vm);
}

} // namespace sn
