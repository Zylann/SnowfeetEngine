#ifndef __HEADER_TGUI_TEXTAREA__
#define __HEADER_TGUI_TEXTAREA__

#include <core/system/time/Time.hpp>

#include "Control.h"
#include "../TextModel.h"

namespace tgui
{

class TextArea : public Control
{
public:
    SN_ENTITY(tgui::TextArea, tgui::Control)

    static const sn::s32 CARET_BLINK_INTERVAL_MS = 500;

    void setCaretIndex(sn::u32 row, sn::u32 column);

    void moveCaretLeft();
    void moveCaretRight();
    void moveCaretUp();
    void moveCaretDown();

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch) override;
    void onSetCursor(Event & ev) override;
    void onMousePress(Event & ev) override;
    void onKeyDown(Event & ev) override;

private:
    void findCaretPositionFromPixelPos(sn::Vector2i pixelPos, sn::Vector2u & out_caretIndex, sn::Vector2i & out_caretPixelPos);
    sn::Vector2i getCaretPositionFromIndex(sn::Vector2u index);

    void clampIndexColumn();
    void clampIndexRow();
    void updateCaretPosition();
    void resetCaretBlink();
    void moveCaretToEndOfLine();

private:
    TextModel m_model;
    sn::Vector2u m_caretIndex; // Column and row
    sn::Vector2i m_caretPosition; // Pixels
    sn::Time m_lastMoveTime; // To reset caret blinking

};

} // namespace tgui

#endif // __HEADER_TGUI_TEXTAREA__

