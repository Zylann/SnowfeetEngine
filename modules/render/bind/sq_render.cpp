#include "sq_render.h"

namespace sn {
namespace render {

void bindRenderModule(HSQUIRRELVM vm)
{
    bindDrawableBase(vm);

    bindDrawable(vm);
    bindCamera(vm);
    bindMaterial(vm);
}

} // namespace render
} // namespace sn
