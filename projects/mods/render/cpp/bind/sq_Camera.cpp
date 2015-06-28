#include <core/app/ScriptableObject.hpp>
#include "../entities/Camera.hpp"
#include "sq_render.hpp"

namespace sn {
namespace render {

void bindCamera(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<sn::render::Camera>(vm);
}

} // namespace render
} // namespace sn

