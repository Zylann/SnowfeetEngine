#ifndef __HEADER_SQ_TGUI__
#define __HEADER_SQ_TGUI__

#include <squirrel.h>
#include <modules/tgui/controls/Panel.h>
#include <modules/tgui/controls/Button.h>
#include <modules/tgui/controls/Slider.h>
#include <modules/tgui/controls/Text.h>

namespace tgui
{

void bindControl(HSQUIRRELVM vm);
void bindButton(HSQUIRRELVM vm);
void bindPanel(HSQUIRRELVM vm);
void bindSlider(HSQUIRRELVM vm);
void bindText(HSQUIRRELVM vm);
void bindGUI(HSQUIRRELVM vm);

void bindTGUI(HSQUIRRELVM vm);

} // namespace tgui

namespace squirrel
{
    template <> inline SQUserPointer getTypeTag<tgui::Control>()  { return tgui::bindControl; }
    template <> inline SQUserPointer getTypeTag<tgui::Button>()   { return tgui::bindButton; }
    template <> inline SQUserPointer getTypeTag<tgui::Panel>()    { return tgui::bindPanel; }
    template <> inline SQUserPointer getTypeTag<tgui::Slider>()   { return tgui::bindSlider; }
    template <> inline SQUserPointer getTypeTag<tgui::Text>()     { return tgui::bindText; }
    template <> inline SQUserPointer getTypeTag<tgui::GUI>()      { return tgui::bindGUI; }
}

#endif // __HEADER_SQ_TGUI__

