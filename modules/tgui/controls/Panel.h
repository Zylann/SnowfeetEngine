#ifndef __HEADER_TGUI_PANEL__
#define __HEADER_TGUI_PANEL__

#include <modules/tgui/controls/Control.h>
#include <modules/tgui/Direction.h>

namespace tgui
{

class SN_TGUI_API Panel : public Control
{
public:
    SN_OBJECT

    Panel();

    bool isResizing() const { return m_resizeDirections.any(); }

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
	void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch) override;

    void onMousePress(Event & e) override;
    void onMouseRelease(Event & e) override;
    void onMouseMove(Event & e) override;
    void onSetCursor(Event & e) override;

private:
	/// \brief Tests if the given cursor position is over a sizing area of the Panel.
    bool checkResizing(sn::Vector2i cursorPos, std::bitset<TGUI_DIRECTION_COUNT> & out_dirs);

private:
    // TODO Move resizing to a Frame container widget?
    bool m_isResizeable;
    bool m_isDraggable;
    sn::s32 m_sideResizerSize;
    sn::s32 m_cornerResizerSize;
    std::bitset<TGUI_DIRECTION_COUNT> m_resizeDirections;

};

} // namespace tgui

#endif // __HEADER_TGUI_PANEL__

