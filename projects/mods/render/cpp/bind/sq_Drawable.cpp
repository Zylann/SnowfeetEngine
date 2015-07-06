#include <core/app/ScriptableObject.h>
#include "../entities/Drawable.h"
#include "sq_render.h"

namespace sn {
namespace render {

void bindDrawable(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<sn::render::Drawable>(vm);
}

} // namespace render
} // namespace sn

