#include <core/app/ScriptableObject.h>
#include "../entities/Camera.h"
#include "sq_render.h"

namespace sn
{

void bindCamera(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<sn::Camera>(vm);
}

} // namespace sn

