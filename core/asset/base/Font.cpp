#include "Font.hpp"

namespace sn
{

s32 Font::getLineWidth(const char * str, u32 charCount, FontFormat format) const
{
    SN_ASSERT(str != nullptr, "Received null string");
    s32 width = 0;
    for (u32 i = 0; i < charCount; ++i)
    {
        char c = str[i];
        const Glyph & glyph = getGlyph(c, format);
        width += glyph.advance;
    }
    return width;
}

} // namespace sn

