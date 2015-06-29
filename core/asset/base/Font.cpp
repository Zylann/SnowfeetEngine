#include "Font.h"

namespace sn
{

s32 Font::getLineWidth(const char * str, u32 charCount, FontFormat format) const
{
    SN_ASSERT(str != nullptr, "Received null string");
    if (charCount == 0)
        return 0;

    s32 width = 0;

#ifdef SN_BUILD_DEBUG
    char lastC = str[0];
#endif

    for (u32 i = 0; i < charCount; ++i)
    {
        char c = str[i];

#ifdef SN_BUILD_DEBUG
        SN_ASSERT(lastC != '\0', "Passed null character, charCount " << charCount << " may be invalid");
        lastC = c;
#endif

        const Glyph & glyph = getGlyph(c, format);
        width += glyph.advance;
    }

    return width;
}

} // namespace sn

