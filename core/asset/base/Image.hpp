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
    const u8 * getPixelData() { return m_pixelData; }

    bool getPixel(u32 x, u32 y, Color8 & out_color) const;
    bool setPixel(u32 x, u32 y, Color8 c);

    const u8 * getPixelsPtr() const { return m_pixelData; }

    u32 getPixelIndex(u32 x, u32 y) const { return x + m_size.x() * y; }

    void fill(Color8 color);

private:
    ~Image();

private:
    u8 * m_pixelData;
    Vector2u m_size;
    PixelFormat m_pixelFormat;

};

} // namespace sn

#endif // __HEADER_SN_IMAGE__

