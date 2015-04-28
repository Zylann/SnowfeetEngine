#ifndef __HEADER_TGUI_BUTTON__
#define __HEADER_TGUI_BUTTON__

#include "Control.hpp"

namespace tgui
{

class Button : public Control
{
public:
    SN_ENTITY(tgui::Button, sn::Control)

    virtual void onMouseEnter(Event & e) {}
    virtual void onMouseLeave(Event & e) {}
    virtual void onMousePress(Event & e) {}
    virtual void onMouseRelease(Event & e) {}

    virtual void onPress() {}
    virtual void onRelease() {}

};

} // namespace tgui

#endif // __HEADER_TGUI_BUTTON__

