#ifndef __HEADER_FREETYPE_FONT__
#define __HEADER_FREETYPE_FONT__

#include <core/asset/base/Font.hpp>
#include <core/util/ShelfPacker.hpp>

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
    const sn::TextureBase * getTexture(sn::FontFormat format) const override;
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

    void setFace(FT_Face face);

protected:
    ~Font();

private:
    void clearFace();

    bool generateGlyph(sn::Glyph & out_glyph, sn::u32 unicode, sn::FontFormat format) const;

    /// \brief FT_Set_Pixel_Sizes is an expensive function, so we must call it
    /// only when necessary to avoid killing performances.
    bool setCurrentSize(sn::u32 characterSize) const;

private:
    typedef std::unordered_map<sn::u32, sn::Glyph> GlyphTable;

    FT_Face                                 m_face;
    mutable sn::Image *                     m_image;
    mutable sn::TextureBase *               m_texture;
    mutable sn::ShelfPacker<sn::Glyph>      m_packer;
    mutable GlyphTable                      m_glyphes[sn::FontFormat::STYLE_COMBINATION_COUNT];

};

} // namespace freetype

#endif // __HEADER_FREETYPE_FONT__

