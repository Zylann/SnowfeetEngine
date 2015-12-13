#ifndef __HEADER_TGUI_TEXTAREA__
#define __HEADER_TGUI_TEXTAREA__

#include <core/system/Time.h>

#include "Control.h"
#include "../TextModel.h"
#include "../TextWrapper.h"

namespace tgui
{

class TextArea : public Control
{
public:
    SN_OBJECT(tgui::TextArea, tgui::Control)

    static const sn::s32 CARET_BLINK_INTERVAL_MS = 500;

    TextArea();

    //void setCaretIndex(sn::u32 row, sn::u32 column);

    void moveCaretLeft();
    void moveCaretRight();
    void moveCaretUp();
    void moveCaretDown();

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch) override;
    void onSetCursor(Event & ev) override;
    void onMousePress(Event & ev) override;
    void onKeyDown(Event & ev) override;
    void onSizeChanged() override;

private:
    void findCaretPositionFromPixelPos(
        sn::Vector2i pixelPos, 
        sn::Vector2u & out_caretIndex, 
        sn::Vector2i & out_caretPixelPos, 
        sn::u32 & out_wrapIndex
    );

    sn::Vector2i getCaretPositionFromIndex(sn::Vector2u index);

    void clampIndexColumn();
    void clampIndexRow();
    void updateCaretPosition();
    void updateCurrentWrapIndex();
    void resetCaretBlink();
    void moveCaretToEndOfLine();
    void updateWrap();

private:
    /// \brief Where the text is stored
    TextModel m_model;
    
    /// \brief View of the text with wrapping
    TextWrapper m_wrapper;

    /// \brief Caret index within the text.
    /// X is the character number within the string (not wrapped),
    /// Y is the line number within TextModel.
    sn::Vector2u m_caretIndex;

    /// \brief Current visual row of the caret (= wrap index)
    sn::u32 m_currentWrap;

    /// \brief Position of the caret in pixels.
    sn::Vector2i m_caretPosition;

    /// \brief Scene time when the cursor last moved.
    /// Used for cursor blinking.
    sn::Time m_lastMoveTime;

};

} // namespace tgui

#endif // __HEADER_TGUI_TEXTAREA__

