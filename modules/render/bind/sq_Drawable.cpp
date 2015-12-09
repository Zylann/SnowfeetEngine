#include "sq_render.h"

namespace sn
{

void bindDrawable(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Drawable>(vm);
}

} // namespace sn
