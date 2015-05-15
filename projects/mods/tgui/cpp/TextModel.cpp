#include "TextModel.h"
#include <core/util/assert.hpp>

using namespace sn;

namespace tgui
{

void TextModel::setSource(const std::string & sourceText)
{
    m_lines.clear();
    if (sourceText.empty())
        return;
    m_lines.push_back("");
    for (u32 i = 0; i < sourceText.size(); ++i)
    {
        char c = sourceText[i];
        if (c == '\n')
        {
            m_lines.push_back("");
        }
        else if (c != '\r')
        {
            m_lines.back() += c;
        }
    }
}

void TextModel::getSource(std::string & out_text) const
{
    if (m_lines.empty())
        return;
    
    out_text = m_lines[0];

    if (m_lines.size() == 1)
        return;

    for (u32 i = 1; i < m_lines.size(); ++i)
    {
        out_text += '\n';
        out_text += m_lines[i];
    }
}

const std::string & TextModel::getLine(sn::u32 i) const
{
    SN_ASSERT(i < m_lines.size(), "Invalid line index " << i);
    return m_lines[i];
}

} // namespace tgui

