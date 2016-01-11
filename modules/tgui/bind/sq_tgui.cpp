#include "sq_tgui.h"

namespace tgui
{

void bindTGUI(HSQUIRRELVM vm)
{
    bindControl(vm);

	bindGUI(vm);
    bindPanel(vm);
    bindButton(vm);
    bindSlider(vm);
    bindText(vm);
}

} // namespace tgui

