#ifndef __HEADER_SN_FONT_GLYPH__
#define __HEADER_SN_FONT_GLYPH__

#include <core/math/Rect.h>

namespace freetype
{

/// \brief Describes one character of a Font
struct Glyph
{
    Glyph() : advance(0) {}

    /// \brief Offset to apply after printing this glyph
    sn::s32 advance;
    /// \brief coordinates of the glyph within the image holding it
    sn::IntRect imageRect;
    /// \brief Bounds of the glyph relative to the baseline
    sn::IntRect bounds;
};

} // namespace freetype

#endif // __HEADER_SN_FONT_GLYPH__

