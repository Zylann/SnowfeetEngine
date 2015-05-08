#ifndef __HEADER_SN_FONT__
#define __HEADER_SN_FONT__

#include <core/asset/base/TextureBase.hpp>
#include <core/asset/base/FontFormat.hpp>
#include <core/math/Rect.hpp>
#include <unordered_set>

namespace sn
{

//------------------------------------------------------------------------------
/// \brief Describes one character of a Font
struct SN_API Glyph
{
    Glyph() : advance(0) {}

    /// \brief Offset to apply after printing this glyph
    s32 advance;
    /// \brief coordinates of the glyph within the image holding it
    IntRect imageRect;
    /// \brief Bounds of the glyph relative to the baseline
    IntRect bounds;
};

//------------------------------------------------------------------------------
/// \brief Stores glyphs inside pixel buffers for further use in 
/// accelerated text rendering or conversion pipelines.
/// \note This class is designed so fonts can be loaded statically
/// (load all glyphes once) or dynamically (load glyphes as they are requested).
class SN_API Font : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Font, sn::Asset)

    /// \brief Gets glyph description from the font
    /// \param unicode: unicode of the character
    /// \param format: format of the character
    /// \return glyph descriptor. If not found, will be a default glyph.
    virtual const Glyph & getGlyph(u32 unicode, FontFormat format) const = 0;

    /// \brief Gets the texture in which are stored glyphes of the specified format.
    /// \param format: the format
    /// \return A texture, or null if pixels are not stored in graphic memory
    virtual const TextureBase * getTexture(FontFormat format) const = 0;

    /// \brief Gets the image in which are stored glyphes of the specified format.
    /// \param format: the format
    /// \return An image, or null if pixels are not stored in memory
    virtual const Image * getImage(FontFormat format) const = 0;

    /// \brief Gets the height of a line, given a character size
    virtual s32 getLineHeight(u32 charSize) const { return 0; }

    /// \brief Gets the offset that should be applied to the second character in order to look more natural.
    /// \param firstUnicode: unicode of the left character
    /// \param secondUnicode: unicode of the right character
    /// \param charSize: characters size
    /// \param style: characters style
    /// \return Horizontal and vertical kerning in the form of a vector
    virtual Vector2i getKerning(
        u32 firstUnicode, 
        u32 secondUnicode, 
        FontFormat format
    ) const { return Vector2i(); }

};

} // namespace sn

#endif // __HEADER_SN_FONT__

