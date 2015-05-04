#ifndef __HEADER_SN_FONT__
#define __HEADER_SN_FONT__

#include <core/asset/base/FontPage.hpp>

namespace sn
{

//------------------------------------------------------------------------------
enum FontStyle
{
    SN_FONT_REGULAR = 0,
    SN_FONT_BOLD,
    SN_FONT_ITALIC,

    SN_FONT_STYLE_COUNT
};

std::string toString(FontStyle style);

//------------------------------------------------------------------------------
/// \brief Stores glyphs inside pixel buffers for further use in 
/// accelerated text rendering or conversion pipelines.
class SN_API Font : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Font, sn::Asset)

    Font();

    s32 getKerning(u32 firstUnicode, u32 secondUnicode, u32 charSize, FontStyle style) const;

    s32 getLineHeight(u32 charSize) const;

    const FontPage * getPage(u32 charSize, FontStyle style) const;

    void clear();

private:
    ~Font();

private:

    /// \brief [charSize][style] => page
    std::vector< std::vector< FontPage* > > m_pages;

};

} // namespace sn

#endif // __HEADER_SN_FONT__

