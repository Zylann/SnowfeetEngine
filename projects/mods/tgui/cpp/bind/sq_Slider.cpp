#include "../controls/Slider.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Slider)
    }

void bindSlider(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Slider>(vm);
}

} // namespace tgui

