#ifndef __HEADER_TGUI_PANEL__
#define __HEADER_TGUI_PANEL__

#include "Control.h"

namespace tgui
{

class Panel : public Control
{
public:
    SN_ENTITY(tgui::Panel, tgui::Control)

protected:
    void onDrawSelf(DrawBatch & batch) override;

    void onMousePress(Event & e) override;
    void onMouseRelease(Event & e) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_PANEL__

