#include "../Material.h"
#include "sq_render.h"
#include <core/squirrel/bind_macros.h>

namespace sn {
namespace render {

    namespace
    {
        CURRENT_CLASS(Material)

        SQRESULT setFloat(HSQUIRRELVM vm)
        {
            GET_SELF();
            const SQChar* field;
            if (SQ_SUCCEEDED(sq_getstring(vm, 2, &field)))
            {
                SQFloat value;
                if (SQ_SUCCEEDED(sq_getfloat(vm, 3, &value)))
                {
                    self->setParam(field, value);
                }
            }
            return 0;
        }

    }

    void bindMaterial(HSQUIRRELVM vm)
    {
        sn::ScriptableObject::bindBase<Material>(vm)
            .setMethod("setFloat", setFloat, 2, "sf|i")
        ;
    }

} // namespace render
} // namespace sn
