#include "FontPage.hpp"

namespace sn
{

//------------------------------------------------------------------------------
FontPage::FontPage()
{
    m_image = new Image();
}

//------------------------------------------------------------------------------
FontPage::~FontPage()
{
    if (m_image)
        m_image->release();
}

//------------------------------------------------------------------------------
const Glyph & FontPage::getGlyph(u32 unicode) const
{
    auto it = m_glyphes.find(unicode);
    if (it != m_glyphes.end())
        return it->second;
    else
    {
        // TODO Return a special existing glyph instead?
        static const Glyph s_default;
#ifdef SN_BUILD_DEBUG
        SN_ERROR("Cannot find glyph " << unicode << " in FontPage");
#endif
        return s_default;
    }
}

//------------------------------------------------------------------------------
void FontPage::addGlyph(u32 unicode, const Glyph & g)
{
    auto it = m_glyphes.find(unicode);
    if (it != m_glyphes.end())
    {
        SN_WARNING("FontPage: Overwriting glyph " << unicode);
        it->second = g;
    }
    else
    {
        m_glyphes[unicode] = g;
    }
}

} // namespace sn


