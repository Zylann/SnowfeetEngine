#include "Font.hpp"

namespace sn
{

//------------------------------------------------------------------------------
std::string toString(FontStyle style)
{
    switch (style)
    {
    case SN_FONT_REGULAR: return "regular";
    case SN_FONT_BOLD: return "bold";
    case SN_FONT_ITALIC: return "italic";
    default: return "unknown";
    }
}

//------------------------------------------------------------------------------
Font::Font() : Asset()
{
}

//------------------------------------------------------------------------------
Font::~Font()
{
    clear();
}

//------------------------------------------------------------------------------
s32 Font::getKerning(u32 firstUnicode, u32 secondUnicode, u32 charSize, FontStyle style) const
{
    // TODO
    return 0;
}

//------------------------------------------------------------------------------
s32 Font::getLineHeight(u32 charSize) const
{
    // TODO
    return 0;
}

//------------------------------------------------------------------------------
const FontPage * Font::getPage(u32 charSize, FontStyle style) const
{
    if (charSize < m_pages.size())
    {
        const auto & pagesByStyle = m_pages[charSize];
        // Note: styles are never negative, but we should suppress the warning
        if (style < static_cast<s32>(pagesByStyle.size()))
        {
            return pagesByStyle[style];
        }
    }
#ifdef SN_BUILD_DEBUG
    SN_ERROR("Cannot find font page for character size " << charSize << " and " << toString(style) << " style");
#endif
    return nullptr;
}

//------------------------------------------------------------------------------
void Font::clear()
{
    for (auto it = m_pages.begin(); it != m_pages.end(); ++it)
    {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {
            FontPage * page = *it2;
            delete page;
        }
    }

    m_pages.clear();
}

} // namespace sn

