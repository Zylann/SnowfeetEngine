#ifndef __HEADER_FREETYPE_FONT__
#define __HEADER_FREETYPE_FONT__

#include <modules/freetype/FontBase.h>
#include <core/util/ShelfPacker.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace freetype
{

/// \brief Freetype font implementation
class Font : public sn::Font, public sn::NonCopyable
{
public:
    SN_SCRIPT_OBJECT(freetype::Font, sn::Font)

    static const sn::u32 DEFAULT_PAGE_WIDTH = 512;
    static const sn::u32 DEFAULT_PAGE_HEIGHT = 512;

    Font();

    //---------------------------------------
    // Overrides
    //---------------------------------------

    const sn::Glyph & getGlyph(sn::u32 unicode, sn::FontFormat format) const override;
    sn::TextureBase * getTexture(sn::FontFormat format) const override;
    const sn::Image * getImage(sn::FontFormat format) const override;

    sn::Vector2i getKerning(
        sn::u32 firstUnicode,
        sn::u32 secondUnicode,
        sn::FontFormat format
    ) const override;

    sn::s32 getLineHeight(sn::u32 charSize) const override;

    //---------------------------------------
    // Internal
    //---------------------------------------

    void setFace(FT_Face face, char * fileData=nullptr);

protected:
    ~Font();

private:
    void clearFace();

    bool generateGlyph(sn::Glyph & out_glyph, sn::u32 unicode, sn::FontFormat format) const;

    /// \brief FT_Set_Pixel_Sizes is an expensive function, so we must call it
    /// only when necessary to avoid killing performances.
    bool setCurrentSize(sn::u32 characterSize) const;

    void createImage() const;
    void createTexture() const;

private:
    typedef std::unordered_map<sn::u32, sn::Glyph> GlyphTable;

    /// \brief Freetype font face
    FT_Face                                 m_face;

    /// \brief Font file data if the face was loaded from memory.
    /// If provided, it will be freed after the font is destroyed.
    char *                                  m_fileData;

    /// \brief Atlassed image in which glyphes will be rasterized
    mutable sn::Image *                     m_image;

    /// \brief VRAM texture in which the image will be uploaded.
    mutable sn::TextureBase *               m_texture;

    /// \brief Packing algorithm used to atlas the glyphes
    mutable sn::ShelfPacker<sn::Glyph>      m_packer;

    /// \brief Glyph informations.
    /// Access: [style][unicode] => Glyph
    mutable GlyphTable                      m_glyphes[sn::FontFormat::STYLE_COMBINATION_COUNT];

};

} // namespace freetype

#endif // __HEADER_FREETYPE_FONT__
