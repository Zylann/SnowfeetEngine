#ifndef __HEADER_SN_FONTFORMAT__
#define __HEADER_SN_FONTFORMAT__

#include <string>
#include <core/types.hpp>

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

    FontFormat() :
        size(12),
        style(STYLE_REGULAR)
    {}

    bool isBold() const { return (style & STYLE_BOLD) != 0; }
    bool isItalic() const { return (style & STYLE_ITALIC) != 0; }
};

std::string toString(FontFormat format);

} // namespace sn

#endif // __HEADER_SN_FONTFORMAT__


