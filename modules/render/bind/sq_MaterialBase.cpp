#include <modules/render/MaterialBase.h>
#include <core/squirrel/bind_macros.h>

namespace sn {
namespace render {

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

} // namespace render
} // namespace sn
