#include <core/app/ScriptableObject.h>
#include "../entities/MeshEntity.h"
#include "sq_render.h"
#include <core/squirrel/bind_macros.h>

namespace sn {
namespace render {

    namespace
    {
        CURRENT_CLASS(MeshEntity)

        BEGIN_METHOD(getMaterial)
            Material * m = self->getMaterial();
            if (m)
                m->pushScriptObject(vm);
            else
                sq_pushnull(vm);
            return 1;
        END_METHOD
    }

void bindMeshEntity(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<MeshEntity>(vm)
        .setMethod("getMaterial", getMaterial)
    ;
}

} // namespace render
} // namespace sn

