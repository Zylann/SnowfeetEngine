#include "../controls/Slider.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

using namespace sn;

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Slider)

        SQRESULT setValue(HSQUIRRELVM vm)
        {
            GET_SELF();
            f32 v = squirrel::getFloat(vm, 2);
            self->setValue(v);
            return 0;
        }

        SQRESULT getValue(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushfloat(vm, self->getValue());
            return 1;
        }
    }

void bindSlider(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Slider>(vm)
        .setMethod("setValue", setValue)
        .setMethod("getValue", getValue);
}

} // namespace tgui

