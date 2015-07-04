#include "sq_tgui.h"

namespace tgui
{

void bindTGUI(HSQUIRRELVM vm)
{
    bindControl(vm);
    bindPanel(vm);
    bindButton(vm);
    bindSlider(vm);
}

} // namespace tgui

