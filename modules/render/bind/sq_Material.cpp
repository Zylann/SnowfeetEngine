#include "../Material.h"
#include "sq_render.h"
#include <core/squirrel/bind_macros.h>

namespace sn {
namespace render {

    //namespace
    //{
    //    CURRENT_CLASS(sn::render::Material)

    //}

    void bindMaterial(HSQUIRRELVM vm)
    {
        sn::ScriptableObject::bindBase<sn::render::Material>(vm);
    }

} // namespace render
} // namespace sn
