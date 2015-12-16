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

        SQRESULT getMin(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushfloat(vm, self->getMin());
            return 1;
        }

        SQRESULT setMin(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->setMin(squirrel::getFloat(vm, 2));
            return 0;
        }

        SQRESULT getMax(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushfloat(vm, self->getMax());
            return 1;
        }

        SQRESULT setMax(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->setMax(squirrel::getFloat(vm, 2));
            return 0;
        }

        SQRESULT getStep(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushfloat(vm, self->getStep());
            return 1;
        }

        SQRESULT setStep(HSQUIRRELVM vm)
        {
            GET_SELF();
            self->setStep(squirrel::getFloat(vm, 2));
            return 0;
        }
    }

void bindSlider(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Slider>(vm)
        .setMethod("setValue", setValue, 2, "xn")
        .setMethod("getValue", getValue)
        .setMethod("setMin", setMin, 2, "xn")
        .setMethod("getMin", getMin)
        .setMethod("setMax", setMax, 2, "xn")
        .setMethod("getMax", getMax)
        .setMethod("setStep", setStep, 2, "xn")
        .setMethod("getStep", getStep)
    ;
}

} // namespace tgui

