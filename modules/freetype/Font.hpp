#ifndef __HEADER_FREETYPE_FONT__
#define __HEADER_FREETYPE_FONT__

#include <core/util/ShelfPacker.h>

#include <modules/render/Texture.h>
#include <modules/freetype/FontFormat.h>
#include <modules/freetype/Glyph.h>

namespace sn
{

/// \brief Stores glyphs inside pixel buffers for further use in 
/// accelerated text rendering or conversion pipelines.
/// \note This class is designed so fonts can be loaded statically
/// (load all glyphes once) or dynamically (load glyphes as they are requested).
class SN_FREETYPE_API Font : public sn::Asset, public sn::NonCopyable
{
public:
    SN_OBJECT

    static const sn::u32 DEFAULT_PAGE_WIDTH = 512;
    static const sn::u32 DEFAULT_PAGE_HEIGHT = 512;

    Font();

    /// \brief Gets glyph description from the font
    /// \param unicode: unicode of the character
    /// \param format: format of the character
    /// \return glyph descriptor. If not found, will be a default glyph.
    const Glyph & getGlyph(sn::u32 unicode, FontFormat format) const;

    /// \brief Gets the texture in which are stored glyphes of the specified format.
    /// \param format: the format
    /// \return A texture, or null if pixels are not stored in graphic memory
    sn::Texture * getTexture(FontFormat format) const;

    /// \brief Gets the image in which are stored glyphes of the specified format.
    /// \param format: the format
    /// \return An image, or null if pixels are not stored in memory
    const sn::Image * getImage(FontFormat format) const;

    /// \brief Gets the height of a line, given a character size
    sn::s32 getLineHeight(sn::u32 charSize) const;

    /// \brief Gets the offset that should be applied to the second character in order to look more natural.
    /// \param firstUnicode: unicode of the left character
    /// \param secondUnicode: unicode of the right character
    /// \param charSize: characters size
    /// \param style: characters style
    /// \return Horizontal and vertical kerning in the form of a vector
    sn::Vector2i getKerning(
        sn::u32 firstUnicode,
        sn::u32 secondUnicode,
        FontFormat format
    ) const;

    /// \brief Calculates the total length of a series of characters.
    /// \warning New line characters are not counted.
    /// \param str: pointer to the first character
    /// \param charCount: number of characters in the string
    /// \param format: font format
    /// \return size in pixels
    sn::s32 getLineWidth(const char * str, sn::u32 charCount, FontFormat format) const;

protected:
    ~Font();

private:
    friend class FontLoader;

    void setFace(void * face, char * fileData=nullptr);
    void clearFace();

    bool generateGlyph(Glyph & out_glyph, sn::u32 unicode, FontFormat format) const;

    /// \brief FT_Set_Pixel_Sizes is an expensive function, so we must call it
    /// only when necessary to avoid killing performances.
    bool setCurrentSize(sn::u32 characterSize) const;

    void createImage() const;
    void createTexture() const;

private:
    typedef std::unordered_map<sn::u32, Glyph> GlyphTable;

    /// \brief Freetype font face
    void * m_face; // FT_Face

    /// \brief Font file data if the face was loaded from memory.
    /// If provided, it will be freed after the font is destroyed.
    char * m_fileData;

    /// \brief Atlassed image in which glyphes will be rasterized
    mutable sn::Image * m_image;

    /// \brief VRAM texture in which the image will be uploaded.
    mutable sn::Texture* m_texture;

    /// \brief Packing algorithm used to atlas the glyphes
    mutable sn::ShelfPacker<Glyph> m_packer;

    /// \brief Glyph informations.
    /// Access: [style][unicode] => Glyph
    mutable GlyphTable m_glyphes[FontFormat::STYLE_COMBINATION_COUNT];

};

} // namespace sn

#endif // __HEADER_FREETYPE_FONT__
