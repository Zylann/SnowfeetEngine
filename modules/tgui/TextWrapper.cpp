#include "TextWrapper.h"
#include <core/util/stringutils.h>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
void serialize(Variant & o, TextWrapMode m)
{
    switch (m)
    {
    case TGUI_WRAP_WORDS: o = "word"; break;
    case TGUI_WRAP_CHARACTERS: o = "character"; break;
    default: o = "none"; break;
    }
}

//------------------------------------------------------------------------------
void unserialize(const Variant & o, TextWrapMode & m)
{
    const std::string & s = o.getString();
    if (s == "word")
        m = TGUI_WRAP_WORDS;
    else if (s == "character")
        m = TGUI_WRAP_CHARACTERS;
    else if (s == "none")
        m = TGUI_WRAP_NONE;
}

//------------------------------------------------------------------------------
void TextWrapper::updateNoWrap()
{
    m_wraps.resize(r_model.getLineCount());
    for (u32 j = 0; j < r_model.getLineCount(); ++j)
    {
        m_wraps[j] = { j, r_model.getLine(j).size() };
    }
}

//------------------------------------------------------------------------------
void TextWrapper::update(sn::u32 width, const Font & font, const FontFormat & format)
{
    if (m_wrapMode == TGUI_WRAP_NONE)
    {
        updateNoWrap();
        return;
    }

    // TODO Implement word-based wrapping

    m_wraps.resize(r_model.getLineCount());
    u32 wrapCount = 0;

    for (u32 j = 0; j < r_model.getLineCount(); ++j)
    {
        const std::string & str = r_model.getLine(j);
        u32 x = 0;

        for (u32 i = 0; i < str.size(); ++i)
        {
            char c = str[i];
            if (isEOL(c))
                break;
            const Glyph & glyph = font.getGlyph(c, format);
            u32 offset = glyph.advance;
            x += offset;
            if (x > width)
            {
                if (m_wraps.size() == wrapCount)
                    m_wraps.push_back({ j, i });
                else
                    m_wraps[wrapCount] = { j, i };
                ++wrapCount;
                x = 0;
            }
        }

        if (m_wraps.size() == wrapCount)
            m_wraps.push_back({ j, str.size() });
        else
            m_wraps[wrapCount] = { j, str.size() };
        ++wrapCount;
    }
}

//------------------------------------------------------------------------------
const TextWrapper::LightWrap & TextWrapper::getLightWrap(sn::u32 visualRow) const
{
    SN_ASSERT(visualRow < m_wraps.size(), "Invalid wrap index " << visualRow);
    return m_wraps[visualRow];
}

//------------------------------------------------------------------------------
TextWrapper::Wrap TextWrapper::getWrap(sn::u32 visualRow) const
{
    //SN_ASSERT(visualRow < m_wraps.size(), "Invalid line index " << visualRow);
    const LightWrap & wrap = getLightWrap(visualRow);
    if (visualRow == 0)
    {
        return{ 0, 0, wrap.end };
    }
    else
    {
        const LightWrap & previousWrap = getLightWrap(visualRow - 1);
        if (previousWrap.line == wrap.line)
        {
            return{ wrap.line, previousWrap.end, wrap.end };
        }
        else
        {
            return{ wrap.line, 0, wrap.end };
        }
    }
}

//------------------------------------------------------------------------------
u32 TextWrapper::getWrapFromLine(u32 lineIndex) const
{
    if (m_wraps.empty())
        return 0;
    
    // Wrap count will always be equal or greater than the actual line count,
    // So we can optimize and start right at the line.
    u32 i = lineIndex;

    // Iterate through next wraps until we find one having the corresponding line number
    for (; i < m_wraps.size(); ++i)
    {
        if (m_wraps[i].line == lineIndex)
            return i;
    }

    // We didn't found the line, so simply return the last wrap
    return m_wraps.size() - 1;
}

//------------------------------------------------------------------------------
sn::u32 TextWrapper::getWrapFromLineAndColumn(sn::u32 line, sn::u32 column) const
{
    if (m_wraps.empty())
        return 0;

    u32 i = getWrapFromLine(line);

    while (m_wraps[i].end < column && i < m_wraps.size())
        ++i;

    return i == m_wraps.size() ? i - 1 : i;
}

//------------------------------------------------------------------------------
u32 TextWrapper::getEndWrap(u32 beginWrap) const
{
    SN_ASSERT(beginWrap < m_wraps.size(), "Invalid wrap index " << beginWrap);
    u32 endWrap = beginWrap+1;
    if (endWrap == m_wraps.size())
        return beginWrap;
    for (; endWrap < m_wraps.size(); ++endWrap)
    {
        if (m_wraps[endWrap].line != m_wraps[beginWrap].line)
        {
            --endWrap;
            break;
        }
    }
    return endWrap;
}

} // namespace tgui

