#include "sq_render.h"

namespace sn {
namespace render {

void bindRenderModule(HSQUIRRELVM vm)
{
    bindDrawable(vm);
    bindCamera(vm);
}

} // namespace render
} // namespace sn

