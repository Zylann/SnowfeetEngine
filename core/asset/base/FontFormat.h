#ifndef __HEADER_SN_FONTFORMAT__
#define __HEADER_SN_FONTFORMAT__

#include <string>
#include <core/types.h>
#include <core/json/json_serialize.h>
#include <core/sml/variant_serialize.h>

namespace sn
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

    u32 size;
    u32 style;

    FontFormat(u32 a_size=16, u32 a_style=STYLE_REGULAR) :
        size(a_size),
        style(a_style)
    {}

    bool isBold() const { return (style & STYLE_BOLD) != 0; }
    bool isItalic() const { return (style & STYLE_ITALIC) != 0; }
};

std::string SN_API toString(FontFormat format);

void SN_API serialize(JsonBox::Value & o, const FontFormat & format);
void SN_API unserialize(const Variant & o, FontFormat & format);

} // namespace sn

#endif // __HEADER_SN_FONTFORMAT__


