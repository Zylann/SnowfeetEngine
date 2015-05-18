#include "TextArea.h"
#include <core/system/gui/SystemGUI.hpp>
#include <core/scene/Scene.hpp>
#include <core/util/stringutils.hpp>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
TextArea::TextArea():
    m_wrapper(m_model),
    m_currentWrap(0)
{
}

//------------------------------------------------------------------------------
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
        m_wrapper,
        bounds,
        *font,
        theme->textFormat,
        TGUI_ALIGN_LEFT,
        theme->textColor
    );

    // Draw caret
    Time time = getScene()->getTimeSinceStartup();
    bool blink = ((time - m_lastMoveTime).asMilliseconds() % (2*CARET_BLINK_INTERVAL_MS)) < CARET_BLINK_INTERVAL_MS;
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

	// DEBUG
	//std::stringstream ss;
	//ss << "Caret: " << sn::toString(m_caretIndex) << ", row: " << m_currentWrap;
	//std::string s = ss.str();
	//batch.drawTextLine(s.c_str(), s.size(), IntRect::fromPositionSize(300, 10, 200, 30), *font, format, TGUI_ALIGN_LEFT, theme->textColor);
}

//------------------------------------------------------------------------------
void TextArea::onSetCursor(Event & ev)
{
    sn::Window * win = SystemGUI::get().getWindowByID(getWindowID());
    if (win)
    {
        win->setMouseCursor(SN_CURSOR_TEXT_EDIT);
        ev.consume();
    }
}

//------------------------------------------------------------------------------
void TextArea::onMousePress(Event & ev)
{
    Vector2i pos(ev.value.mouse.x, ev.value.mouse.y);
    pos -= getPosition();

    resetCaretBlink();

    findCaretPositionFromPixelPos(pos, m_caretIndex, m_caretPosition, m_currentWrap);

    ev.consume();
}

//------------------------------------------------------------------------------
void TextArea::onKeyDown(Event & ev)
{
    KeyCode key = ev.value.keyboard.keyCode;
    switch (key)
    {
    case SN_KEY_LEFT:
        moveCaretLeft();
        break;

    case SN_KEY_RIGHT:
        moveCaretRight();
        break;

    case SN_KEY_UP:
        moveCaretUp();
        break;

    case SN_KEY_DOWN:
        moveCaretDown();
        break;

    //case SN_KEY_DELETE:
    //    break;

    //case SN_KEY_BACKSPACE:
    //    break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------
void TextArea::onSizeChanged()
{
    updateWrap();
}

//------------------------------------------------------------------------------
void TextArea::moveCaretLeft()
{
    if (m_model.getLineCount() == 0)
        return;

    TextWrapper::Wrap wrap = m_wrapper.getWrap(m_currentWrap);

    if (m_caretIndex.x() > 0)
    {
        --m_caretIndex.x();
        if (m_caretIndex.x() < wrap.begin)
            --m_currentWrap;
        updateCaretPosition();
    }
    else if (m_caretIndex.y() > 0)
    {
        --m_caretIndex.y();
        --m_currentWrap;
        moveCaretToEndOfLine();
        updateCaretPosition();
    }
}

//------------------------------------------------------------------------------
void TextArea::moveCaretRight()
{
    if (m_model.getLineCount() == 0)
        return;
    
    TextWrapper::Wrap wrap = m_wrapper.getWrap(m_currentWrap);
    const std::string & line = m_model.getLine(wrap.line);

    if (m_caretIndex.x() < line.size() && !isEOL(line[m_caretIndex.x()]))
    {
        ++m_caretIndex.x();
        clampIndexColumn();
        if (m_caretIndex.x() >= wrap.end && m_currentWrap+1 < m_wrapper.getWrapCount())
            ++m_currentWrap;
        updateCaretPosition();
    }
    else if (m_caretIndex.y() + 1 < m_model.getLineCount())
    {
        m_caretIndex.x() = 0;
        ++m_caretIndex.y();
        ++m_currentWrap;
        updateCaretPosition();
    }
}

//------------------------------------------------------------------------------
void TextArea::moveCaretUp()
{
    if (m_currentWrap > 0)
    {
        TextWrapper::Wrap lastWrap = m_wrapper.getWrap(m_currentWrap);
        --m_currentWrap;
        TextWrapper::Wrap wrap = m_wrapper.getWrap(m_currentWrap);

        if (lastWrap.line == wrap.line)
        {
            // We are still in the same line
            m_caretIndex.x() -= wrap.end;
        }
        else
        {
            // We moved to the previous line
            --m_caretIndex.y();
            m_caretIndex.x() += wrap.begin;
        }

		// Make sure we don't go out of line bounds
		if (m_caretIndex.x() >= wrap.end)
			m_caretIndex.x() = wrap.end;

        updateCaretPosition();
    }
}

//------------------------------------------------------------------------------
void TextArea::moveCaretDown()
{
    if (m_currentWrap + 1 < m_wrapper.getWrapCount())
    {
        TextWrapper::Wrap lastWrap = m_wrapper.getWrap(m_currentWrap);
        ++m_currentWrap;
        TextWrapper::Wrap wrap = m_wrapper.getWrap(m_currentWrap);

        if (lastWrap.line == wrap.line)
        {
            // We are still in the same line
            m_caretIndex.x() += lastWrap.end;
        }
        else
        {
            // We moved to the next line
            ++m_caretIndex.y();
        }

		// Make sure we don't go out of line bounds
		if (m_caretIndex.x() >= wrap.end)
			m_caretIndex.x() = wrap.end;

        updateCaretPosition();
    }

    SN_LOG("Caret index: " << sn::toString(m_caretIndex) << ", row: " << m_currentWrap);
}

//------------------------------------------------------------------------------
void TextArea::moveCaretToEndOfLine()
{
    if (m_model.getLineCount() == 0)
    {
        // Nothing in the model, put the caret at the origin
        m_caretIndex.x() = 0;
    }
    else
    {
        // Move to the last wrap covering the current line
        m_currentWrap = m_wrapper.getEndWrap(m_currentWrap);

        TextWrapper::Wrap wrap = m_wrapper.getWrap(m_currentWrap);
        const std::string & str = m_model.getLine(wrap.line);

        if (str.size() == 0)
        {
            // The current line is empty, put the caret at its beginning
            m_caretIndex.x() = 0;
        }
        else
        {
            // If the line has no newline character, place the caret at the end index.
            u32 i = str.size();

            // If the line has newline characters, go back to the first one
            while (isEOL(str[i - 1]) && i > 0)
            {
                --i;
            }

            m_caretIndex.x() = i;
        }
    }
}

//------------------------------------------------------------------------------
//void TextArea::setCaretIndex(sn::u32 row, sn::u32 column)
//{
//    m_caretIndex.x() = column;
//    clampIndexColumn();
//    
//    m_caretIndex.y() = row;
//    clampIndexRow();
//
//    updateCaretPosition();
//}

//------------------------------------------------------------------------------
void TextArea::clampIndexColumn()
{
    const TextWrapper::LightWrap & wrap = m_wrapper.getLightWrap(m_currentWrap);
    u32 lineSize = m_model.getLine(wrap.line).size();
    if (m_caretIndex.x() >= lineSize)
        moveCaretToEndOfLine();
}

//------------------------------------------------------------------------------
void TextArea::clampIndexRow()
{
    u32 lineCount = m_model.getLineCount();
    if (m_caretIndex.y() >= lineCount)
    {
        m_caretIndex.y() = lineCount == 0 ? 0 : lineCount - 1;
        m_currentWrap = m_wrapper.getWrapCount() - 1;
    }
}

//------------------------------------------------------------------------------
void TextArea::updateCurrentWrapIndex()
{
    m_currentWrap = m_wrapper.getWrapFromLineAndColumn(m_caretIndex.y(), m_caretIndex.x());
}

//------------------------------------------------------------------------------
void TextArea::updateCaretPosition()
{
    m_caretPosition = getCaretPositionFromIndex(m_caretIndex);
    resetCaretBlink();
}

//------------------------------------------------------------------------------
sn::Vector2i TextArea::getCaretPositionFromIndex(sn::Vector2u index)
{
    Vector2i pos;

    // Get the font
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return pos;
    const Font * font = theme->getFont();
    if (font == nullptr)
        return pos;

    const FontFormat & format = theme->textFormat;
    s32 lineHeight = font->getLineHeight(format.size);

    // Get Y
    pos.y() = m_currentWrap * lineHeight;

    // Get X
    const std::string & str = m_model.getLine(index.y());
    const auto & wrap = m_wrapper.getWrap(m_currentWrap);
    pos.x() = font->getLineWidth(str.c_str() + wrap.begin, index.x() - wrap.begin, format);

    return pos;
}

//------------------------------------------------------------------------------
void TextArea::findCaretPositionFromPixelPos(Vector2i pixelPos, Vector2u & out_caretIndex, Vector2i & out_caretPixelPos, u32 & out_wrapIndex)
{
    // Top left by default
    out_caretIndex = Vector2u(0, 0);
    out_caretPixelPos = Vector2i(0, 0);

    // Do nothing if there is no text
    if (m_model.getLineCount() == 0 || m_wrapper.getWrapCount() == 0)
        return;

    // Get the font
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    const Font * font = theme->getFont();
    if (font == nullptr)
        return;

    // Get format
    const FontFormat & format = theme->textFormat;
    s32 lineHeight = font->getLineHeight(format.size);

    // If the line height is zero...
    if (lineHeight <= 0)
        return;

    // Find visual row
    u32 visualRow = math::clamp(pixelPos.y() / lineHeight, 0, static_cast<s32>(m_wrapper.getWrapCount())-1);
    const auto & wrap = m_wrapper.getWrap(visualRow);
    out_wrapIndex = visualRow;

    // Find Y
    out_caretIndex.y() = wrap.line;
    out_caretPixelPos.y() = visualRow * lineHeight;

    // Find column number and X coordinate
    if (pixelPos.x() > 0)
    {
        const std::string & str = m_model.getLine(wrap.line);

        u32 column = wrap.begin;
        s32 cx = 0;

        for (; column < str.size(); ++column)
        {
            char c = str[column];
            if (c == '\n' || c == '\r')
                break;
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

//------------------------------------------------------------------------------
void TextArea::resetCaretBlink()
{
    m_lastMoveTime = getScene()->getTimeSinceStartup();
}

//------------------------------------------------------------------------------
void TextArea::updateWrap()
{
    // Get the font
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    const Font * font = theme->getFont();
    if (font == nullptr)
        return;

    m_wrapper.update(getLocalClientBounds().width(), *font, theme->textFormat);
    updateCurrentWrapIndex();
    updateCaretPosition();
}

//------------------------------------------------------------------------------
void TextArea::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

    std::string text;
    m_model.getSource(text);
    sn::serialize(o["text"], text);
}

//------------------------------------------------------------------------------
void TextArea::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

    std::string text;
    sn::unserialize(o["text"], text);
    m_model.setSource(text);
}

} // namespace tgui

