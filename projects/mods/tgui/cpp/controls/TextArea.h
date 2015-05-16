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

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch) override;
    void onSetCursor(Event & ev) override;
    void onMousePress(Event & ev) override;

private:
    void findCaretPosition(sn::Vector2i pixelPos, sn::Vector2u & out_caretIndex, sn::Vector2i & out_caretPixelPos);

private:
    TextModel m_model;
    sn::Vector2u m_caretIndex; // Column and row
    sn::Vector2i m_caretPosition; // Pixels
    sn::Time m_lastClickTime; // To reset caret blinking

};

} // namespace tgui

#endif // __HEADER_TGUI_TEXTAREA__

