#ifndef __HEADER_TGUI_TEXTMODEL__
#define __HEADER_TGUI_TEXTMODEL__

#include <vector>
#include <core/types.hpp>
#include <core/asset/base/Font.hpp>

namespace tgui
{

/// \brief Stores actual editable data of a text.
/// The text is stored as lines for better performance at editing.
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

