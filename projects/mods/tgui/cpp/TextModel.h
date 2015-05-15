#ifndef __HEADER_TGUI_TEXTMODEL__
#define __HEADER_TGUI_TEXTMODEL__

#include <vector>
#include <core/types.hpp>

namespace tgui
{

class TextModel
{
public:
    void setSource(const std::string & sourceText);
    void getSource(std::string & out_text) const;

    sn::u32 getLineCount() const { return m_lines.size(); }
    const std::string & getLine(sn::u32 i) const;

private:
    std::vector<std::string> m_lines;
};

} // namespace tgui

#endif // __HEADER_TGUI_TEXTMODEL__

