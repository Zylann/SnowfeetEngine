#ifndef __HEADER_SN_IMAGE__
#define __HEADER_SN_IMAGE__

#include <core/asset/Asset.hpp>
#include <core/math/Vector2.hpp>

namespace sn
{

enum PixelFormat
{
    SN_IMAGE_RGBA32 = 0
};

class SN_API Image : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Image, sn::Asset)

    Image();
    Image(const Image & other);

    void create(
        Vector2u size,
        PixelFormat format=SN_IMAGE_RGBA32, 
        Color8 defaultColor=Color8(255,255,255,255)
    );

    void loadFromPixels(
        Vector2u size,
        PixelFormat format,
        const u8 * pixelData
    );

    void clear();

    Vector2i getSize() const { return m_size; }

    PixelFormat getPixelFormat() const { return m_pixelFormat; }
    // Note: only RGBA is supported at the moment
    u32 getChannelCount() const { return 4; }

    bool getPixel(u32 x, u32 y, Color8 & out_color) const;
    bool setPixel(u32 x, u32 y, Color8 c);

    const u8 * getPixelsPtr() const { return m_pixelData; }

    u32 getPixelIndex(u32 x, u32 y) const { return x + m_size.x() * y; }

    void fill(Color8 color);

    void pasteSubImage(const Image & image, s32 x, s32 y);
    void pasteSubImage(const u8 * pixels, s32 x, s32 y, u32 w, u32 h, PixelFormat format);

    Image & operator=(const Image & other);

private:
    ~Image();

    void createNoFill(Vector2u size, PixelFormat format);
    u32 getDataLength() const { return m_size.x() * m_size.y() * getChannelCount(); }

private:
    u8 * m_pixelData;
    Vector2u m_size;
    PixelFormat m_pixelFormat;

};

} // namespace sn

#endif // __HEADER_SN_IMAGE__

