#include "../controls/Panel.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Panel)
    }

void bindPanel(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Panel>(vm);
}

} // namespace tgui

