#include <core/app/ScriptableObject.h>
#include "../entities/Camera.h"
#include "sq_render.h"

namespace sn {
namespace render {

void bindCamera(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<sn::render::Camera>(vm);
}

} // namespace render
} // namespace sn

