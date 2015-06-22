#include "../controls/Control.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Control)
    }

void bindControl(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Control>(vm);
}

} // namespace tgui

