#include "../controls/Button.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Button)
    }

void bindButton(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Button>(vm);
}

} // namespace tgui

