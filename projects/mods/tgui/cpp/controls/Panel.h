#ifndef __HEADER_TGUI_PANEL__
#define __HEADER_TGUI_PANEL__

#include "Control.h"
#include "../Direction.h"

namespace tgui
{

class Panel : public Control
{
public:
    SN_ENTITY(tgui::Panel, tgui::Control)

    Panel();

    bool isResizing() const { return m_resizeDirections.any(); }

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch) override;

    void onMousePress(Event & e) override;
    void onMouseRelease(Event & e) override;
    void onMouseMove(Event & e) override;
    void onSetCursor(Event & e) override;

private:
    bool checkResizing(sn::Vector2i cursorPos, std::bitset<TGUI_DIRECTION_COUNT> & out_dirs);

private:
    bool m_isResizeable;
    sn::s32 m_sideResizerSize;
    sn::s32 m_cornerResizerSize;
    std::bitset<TGUI_DIRECTION_COUNT> m_resizeDirections;

};

} // namespace tgui

#endif // __HEADER_TGUI_PANEL__

