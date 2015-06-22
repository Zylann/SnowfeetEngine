#ifndef __HEADER_SQ_TGUI__
#define __HEADER_SQ_TGUI__

#include <squirrel.h>

namespace tgui
{

void bindControl(HSQUIRRELVM vm);
void bindButton(HSQUIRRELVM vm);

void bindTGUI(HSQUIRRELVM vm);

} // namespace tgui

#endif // __HEADER_SQ_TGUI__

