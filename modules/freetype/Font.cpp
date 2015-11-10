#include "Font.hpp"
#include <core/asset/AssetDatabase.h>
#include <core/app/Application.h>
#include <modules/render/VideoDriverBase.h>

#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

using namespace sn;

namespace freetype
{

//------------------------------------------------------------------------------
Font::Font() : sn::Font(),
    m_face(nullptr),
    m_fileData(nullptr),
    m_image(nullptr),
    m_texture(nullptr),
    m_packer(DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT)
{
}

//------------------------------------------------------------------------------
Font::~Font()
{
    clearFace();
    if (m_texture != nullptr)
        m_texture->release();
    if (m_image != nullptr)
        m_image->release();
}

//------------------------------------------------------------------------------
const sn::Glyph & Font::getGlyph(sn::u32 unicode, sn::FontFormat format) const
{
    auto & glyphes = m_glyphes[format.style];
    auto it = glyphes.find(unicode);
    if (it == glyphes.end())
    {
        Glyph glyph;
        if (generateGlyph(glyph, unicode, format))
        {
            auto ret = glyphes.insert(std::make_pair(unicode, glyph));
            return ret.first->second;
        }
        else
        {
            static Glyph s_defaultGlyph;
            return s_defaultGlyph;
        }
    }
    return it->second;
}

//------------------------------------------------------------------------------
bool Font::generateGlyph(Glyph & out_glyph, sn::u32 unicode, sn::FontFormat format) const
{
    Glyph glyph;

    if (!setCurrentSize(format.size))
        return false;

    // Load the glyph corresponding to the code point
    if (FT_Load_Char(m_face, unicode, FT_LOAD_TARGET_NORMAL) != 0)
        return false;

    // Retrieve the glyph
    FT_Glyph glyphDesc;
    if (FT_Get_Glyph(m_face->glyph, &glyphDesc) != 0)
        return false;

    // Apply bold if necessary -- first technique using outline (highest quality)
    FT_Pos weight = 1 << 6;
    bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    if (format.isBold() && outline)
    {
        FT_OutlineGlyph outlineGlyph = (FT_OutlineGlyph)glyphDesc;
        FT_Outline_Embolden(&outlineGlyph->outline, weight);
    }

    // Convert the glyph to a bitmap (i.e. rasterize it)
    if (glyphDesc->format != FT_GLYPH_FORMAT_BITMAP)
    {
        if (FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1) != 0)
        {
            SN_ERROR("Failed to convert glyph to bitmap");
        }
    }
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;
    FT_Bitmap& bitmap = bitmapGlyph->bitmap;

    // TODO this technique requires a pointer to the library, which is not handy given the module's design.
    // Apply bold if necessary -- fallback technique using bitmap (lower quality)
    //if (format.isBold() && !outline)
    //{
    //    FT_Bitmap_Embolden(static_cast<FT_Library>(m_library), &bitmap, weight, weight);
    //}

    // Compute the glyph's advance offset
    glyph.advance = glyphDesc->advance.x >> 16;
    if (format.isBold())
        glyph.advance += weight >> 6;

    u32 width  = bitmap.width;
    u32 height = bitmap.rows;
    if ((width > 0) && (height > 0))
    {
        // Leave a small padding around characters, so that filtering doesn't
        // pollute them with pixels from neighbours
        const unsigned int padding = 1;

        ShelfPacker<Glyph>::Node packNode(glyph, width + 2 * padding, height + 2 * padding);
        if (!m_packer.insert(packNode, &glyph.imageRect))
        {
            // TODO Atlas resizing
            SN_WARNING("Cannot insert new glyph in atlas");
            FT_Done_Glyph(glyphDesc);
            return false;
        }

        // Compute the glyph's bounding box
        glyph.bounds = IntRect::fromPositionSize(
            bitmapGlyph->left - padding,
            -bitmapGlyph->top - padding,
            width + 2 * padding,
            height + 2 * padding
        );

        // Extract the glyph's pixels from the bitmap

        // Get the source buffer
        const u8 * src = bitmap.buffer;

        // Create the destination buffer
        const u32 dstSize = width * height * 4;
        u8 * dst = new u8[width * height * 4];
        // Fill with white
        memset(dst, 255, dstSize);

        // Convert from source to destination
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
        {
            // Pixels are 1 bit monochrome values
            for (u32 y = 0; y < height; ++y)
            {
                for (u32 x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    size_t dstIndex = (x + y * width) * 4 + 3;
                    dst[dstIndex] = ((src[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                }
                src += bitmap.pitch;
            }
        }
        else
        {
            // Pixels are 8 bits gray levels
            for (u32 y = 0; y < height; ++y)
            {
                for (u32 x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    size_t dstIndex = (x + y * width) * 4 + 3;
                    dst[dstIndex] = src[x];
                }
                src += bitmap.pitch;
            }
        }

        // Paste pixels into the image
        u32 x = glyph.imageRect.x() + padding;
        u32 y = glyph.imageRect.y() + padding;
        u32 w = width;//glyph.imageRect.width() - 2 * padding;
        u32 h = height;//glyph.imageRect.height() - 2 * padding;
        m_image->pasteSubImage(dst, x, y, w, h, SN_IMAGE_RGBA32);

        if (m_texture)
        {
            // Update the texture
            // TODO Optimization: only update a sub-rectangle of the texture
            m_texture->uploadToVRAM();
        }

        delete[] dst;
    }
    else
    {
        SN_DLOG("Character " << unicode << " (ascii: " << (char)unicode << ") has an empty bitmap");
    }

    // Delete the FT glyph
    FT_Done_Glyph(glyphDesc);

    out_glyph = glyph;

    return true;
}

//------------------------------------------------------------------------------
sn::TextureBase * Font::getTexture(sn::FontFormat format) const
{
    return m_texture;
}

//------------------------------------------------------------------------------
const sn::Image * Font::getImage(sn::FontFormat format) const
{
    return m_image;
}

//------------------------------------------------------------------------------
Vector2i Font::getKerning(u32 firstUnicode, u32 secondUnicode, FontFormat format) const
{
    // Special case where first or second is 0 (null character)
    if (firstUnicode == 0 || secondUnicode == 0)
        return Vector2i();

    if (m_face && FT_HAS_KERNING(m_face) && setCurrentSize(format.size))
    {
        // Convert the characters to indices
        FT_UInt index1 = FT_Get_Char_Index(m_face, firstUnicode);
        FT_UInt index2 = FT_Get_Char_Index(m_face, secondUnicode);

        // Get the kerning vector
        FT_Vector kerning;
        FT_Get_Kerning(m_face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        return Vector2i(kerning.x, kerning.y);
    }
    else
    {
        // Invalid font, or no kerning
        return Vector2i();
    }
}

//------------------------------------------------------------------------------
s32 Font::getLineHeight(u32 charSize) const
{
    if (m_face && setCurrentSize(charSize))
    {
        return m_face->size->metrics.height >> 6;
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
bool Font::setCurrentSize(sn::u32 characterSize) const
{
    SN_ASSERT(m_face != nullptr, "Invalid state: Freetype font face is null");

    FT_UShort currentSize = m_face->size->metrics.x_ppem;

    if (currentSize != characterSize)
    {
        return FT_Set_Pixel_Sizes(m_face, 0, characterSize) == 0;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
void Font::setFace(FT_Face face, char * fileData)
{
    if (m_face != face)
    {
        clearFace();
        m_face = face;
        m_fileData = fileData;

        if (m_face)
        {
            if (m_image == nullptr)
                createImage();

            // TODO Don't do this if we don't want to draw the font
            if (m_texture == nullptr)
                createTexture();
        }
    }
}

//------------------------------------------------------------------------------
void Font::clearFace()
{
    if (m_face)
    {
        FT_Done_Face(static_cast<FT_Face>(m_face));
        m_face = nullptr;
    }
    if (m_fileData)
    {
        delete[] m_fileData;
        m_fileData = nullptr;
    }
}

//------------------------------------------------------------------------------
void Font::createImage() const
{
    SN_ASSERT(m_image == nullptr, "Image already created");
    m_image = new Image();
    m_image->create(
        Vector2u(DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT), 
        sn::SN_IMAGE_RGBA32, 
        sn::Color8(0,0,0,0)
    );
}

//------------------------------------------------------------------------------
void Font::createTexture() const
{
    SN_ASSERT(m_texture == nullptr, "Texture already created");
    SN_ASSERT(m_image != nullptr, "Image not created");
    IVideoDriver * driver = Application::get().getDriverManager().getDriver<IVideoDriver>();
    if (driver)
    {
        m_texture = driver->createTexture();
        m_texture->setSourceImage(*m_image);
        m_texture->setKeepSourceInMemory(true);
    }
}

} // namespace freetype

