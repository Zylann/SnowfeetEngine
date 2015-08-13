#include <core/asset/base/Material.h>
#include "sq_core.h"
#include "../squirrel/bind_macros.h"

namespace sn
{
    namespace
    {
        CURRENT_CLASS(sn::Material)

        BEGIN_METHOD(setFloat)
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
        END_METHOD

    }

    void bindMaterialBase(HSQUIRRELVM vm)
    {
        ScriptableObject::bindBase<Material>(vm)
            .setMethod("setFloat", setFloat, 2, "sf|i");
    }

} // namespace sn
