#include "TextArea.h"
#include <core/system/gui/SystemGUI.hpp>
#include <core/scene/Scene.hpp>

using namespace sn;

namespace tgui
{

void TextArea::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    Font * font = theme->getFont();
    if (font == nullptr)
        return;

    const ControlTheme & bg = theme->textAreaBackground;
    IntRect bounds = getClientBounds();

    // Draw background
    const IntRect & uvRect = bg.statesUV[0];
    Vector2u ts = theme->getTextureAtlasSize();
    batch.fillNineSlices(bounds, bg.slicing, uvRect, ts);

    // Draw content:

    batch.setScissor(bounds, getWindowID());

    // Draw text
    const FontFormat & format = theme->textFormat;
    batch.drawText(
        m_model,
        bounds,
        *font,
        theme->textFormat,
        TGUI_ALIGN_LEFT,
        theme->textColor
    );

    // Draw caret
    bool blink = (m_lastClickTime.asMilliseconds() % CARET_BLINK_INTERVAL_MS) > CARET_BLINK_INTERVAL_MS / 2;
    if (blink)
    {
        const ControlTheme & caretTheme = theme->textAreaCaret;
        batch.fillRect(
            IntRect::fromPositionSize(
                bounds.x() + m_caretPosition.x(), 
                bounds.y() + m_caretPosition.y(), 
                1, font->getLineHeight(format.size)
            ), 
            caretTheme.statesUV[0], ts
        );
    }

    batch.disableScissor();
}

void TextArea::onSetCursor(Event & ev)
{
    sn::Window * win = SystemGUI::get().getWindowByID(getWindowID());
    if (win)
    {
        win->setMouseCursor(SN_CURSOR_TEXT_EDIT);
        ev.consume();
    }
}

void TextArea::onMousePress(Event & ev)
{
    Vector2i pos(ev.value.mouse.x, ev.value.mouse.y);
    pos -= getPosition();

    m_lastClickTime = getScene()->getTimeSinceStartup();

    findCaretPosition(pos, m_caretIndex, m_caretPosition);

    ev.consume();
}

void TextArea::findCaretPosition(Vector2i pixelPos, Vector2u & out_caretIndex, Vector2i & out_caretPixelPos)
{
    // Top left by default
    out_caretIndex = Vector2u(0, 0);
    out_caretPixelPos = Vector2i(0, 0);

    // Do nothing if there is no text
    if (m_model.getLineCount() == 0)
        return;

    // Get the font
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    const Font * font = theme->getFont();
    if (font == nullptr)
        return;

    const FontFormat & format = theme->textFormat;
    s32 lineHeight = font->getLineHeight(format.size);

    // If the line height is zero...
    if (lineHeight <= 0)
        return;

    // Find line number and Y coordinate
    u32 row = math::clamp(pixelPos.y() / lineHeight, 0, static_cast<s32>(m_model.getLineCount())-1);
    out_caretIndex.y() = row;
    out_caretPixelPos.y() = row * lineHeight;

    // Find column number and X coordinate
    if (pixelPos.x() > 0)
    {
        const std::string & str = m_model.getLine(row);

        u32 column = 0;
        s32 cx = 0;

        for (; column < str.size(); ++column)
        {
            char c = str[column];
            const Glyph & glyph = font->getGlyph(c, format);
            s32 offset = glyph.advance;
            if (pixelPos.x() >= cx && pixelPos.x() < cx + offset)
            {
                // Found inline
                break;
            }
            cx += offset;
        }
        
        out_caretIndex.x() = column;
        out_caretPixelPos.x() = cx;
    }
}

void TextArea::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

    std::string text;
    m_model.getSource(text);
    sn::serialize(o["text"], text);
}

void TextArea::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

    std::string text;
    sn::unserialize(o["text"], text);
    m_model.setSource(text);
}

} // namespace tgui

