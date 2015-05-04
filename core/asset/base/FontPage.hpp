#ifndef __HEADER_SN_FONTPAGE__
#define __HEADER_SN_FONTPAGE__

#include <core/asset/base/Image.hpp>
#include <core/math/Rect.hpp>

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
};

//------------------------------------------------------------------------------
class SN_API FontPage
{
public:
    FontPage();
    ~FontPage();

    const Glyph & getGlyph(u32 unicode) const;
    void addGlyph(u32 unicode, const Glyph & g);

    Image & getImage() const { return *m_image; }

private:
    std::unordered_map<u32, Glyph> m_glyphes;
    Image * m_image;
    //Texture * m_texture;
};

} // namespace sn

#endif // __HEADER_SN_FONTPAGE__

