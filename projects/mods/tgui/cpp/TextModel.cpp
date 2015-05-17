#include "TextModel.h"
#include <core/util/assert.hpp>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
void TextModel::setSource(const std::string & sourceText)
{
    m_lines.clear();
    if (sourceText.empty())
        return;
    m_lines.push_back("");
    for (u32 i = 0; i < sourceText.size(); ++i)
    {
        char c = sourceText[i];
        if (c != '\r')
        {
            m_lines.back() += c;
        }
        if (c == '\n')
        {
            m_lines.push_back("");
        }
    }
}

//------------------------------------------------------------------------------
void TextModel::getSource(std::string & out_text) const
{
    if (m_lines.empty())
        return;
    
    out_text = m_lines[0];

    if (m_lines.size() == 1)
        return;

    for (u32 i = 1; i < m_lines.size(); ++i)
    {
        // Note: EOL characters are already included in lines
        out_text += m_lines[i];
    }
}

//------------------------------------------------------------------------------
const std::string & TextModel::getLine(sn::u32 i) const
{
    SN_ASSERT(i < m_lines.size(), "Invalid line index " << i);
    return m_lines[i];
}

//------------------------------------------------------------------------------
////////////////////////////////////
//void TextModel::wrap(
//    u32 row,
//    TextWrapMode mode,
//    const sn::Font & font,
//    const sn::FontFormat & format,
//    const sn::IntRect & rect
//)
////////////////////////////////////
//{
//    SN_ASSERT(row < m_lines.size(), "Invalid line index " << row);
//
//    LineModel & line = m_lines[row];
//    s32 lineWidth = 0;
//    std::vector<u32> wraps;
//    wraps.push_back(0);
//
//    if (mode != TGUI_WRAP_NONE)
//    {
//        for (u32 i = 0; i < line.str.size(); ++i)
//        {
//            char c = line.str[i];
//            const Glyph & glyph = font.getGlyph(c, format);
//            u32 offset = glyph.advance;
//            lineWidth += offset;
//            if (lineWidth >= rect.width())
//            {
//                if (mode == TGUI_WRAP_CHARACTERS)
//                {
//                    wraps.push_back(i);
//                    lineWidth = 0;
//                }
//                else
//                {
//                    // TODO
//                }
//            }
//        }
//    }
//
//    wraps.push_back(line.str.size());
//    line.wraps = wraps;
//}

} // namespace tgui

