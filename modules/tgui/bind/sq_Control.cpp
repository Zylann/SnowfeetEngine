#include "../controls/Control.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Control)

        SQRESULT isPressed(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushbool(vm, self->isPressed());
            return 1;
        }
    }

void bindControl(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Control>(vm)
        .setMethod("isPressed", isPressed)
    ;
}

} // namespace tgui

