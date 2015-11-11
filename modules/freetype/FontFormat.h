#ifndef __HEADER_SN_FONTFORMAT__
#define __HEADER_SN_FONTFORMAT__

#include <core/types.h>
#include <core/sml/variant_serialize.h>

#include <modules/freetype/common.h>

namespace freetype
{

struct FontFormat
{
    enum Style
    {
        STYLE_REGULAR = 0,
        STYLE_BOLD = 1,
        STYLE_ITALIC = 2,

        STYLE_COMBINATION_COUNT = STYLE_BOLD | STYLE_ITALIC + 1
    };

    sn::u32 size;
    sn::u32 style;

    FontFormat(sn::u32 a_size=16, sn::u32 a_style=STYLE_REGULAR) :
        size(a_size),
        style(a_style)
    {}

    bool isBold() const { return (style & STYLE_BOLD) != 0; }
    bool isItalic() const { return (style & STYLE_ITALIC) != 0; }
};

SN_FREETYPE_API std::string toString(FontFormat format);

SN_FREETYPE_API void serialize(sn::Variant & o, const FontFormat & format);
SN_FREETYPE_API void unserialize(const sn::Variant & o, FontFormat & format);

} // namespace freetype

#endif // __HEADER_SN_FONTFORMAT__


