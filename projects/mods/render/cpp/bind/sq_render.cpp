#include "sq_render.hpp"

namespace sn {
namespace render {

void bindRenderModule(HSQUIRRELVM vm)
{
    bindDrawable(vm);
    bindCamera(vm);
}

} // namespace render
} // namespace sn

