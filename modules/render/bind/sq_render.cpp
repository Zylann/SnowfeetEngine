#include "sq_render.h"

namespace sn {
namespace render {

void bindRenderModule(HSQUIRRELVM vm)
{
    bindDrawable(vm);
    bindMeshEntity(vm);
    bindCamera(vm);
    bindMaterial(vm);
}

} // namespace render
} // namespace sn
