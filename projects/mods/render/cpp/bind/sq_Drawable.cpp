#include <core/app/ScriptableObject.h>
#include "../entities/Drawable.hpp"
#include "sq_render.hpp"

namespace sn {
namespace render {

void bindDrawable(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<sn::render::Drawable>(vm);
}

} // namespace render
} // namespace sn

