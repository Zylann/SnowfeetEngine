#include "Image.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Image::Image() : Asset(),
    m_pixelData(nullptr),
    m_pixelFormat(SN_IMAGE_RGBA32) // Not really relevant, but known as default
{
}

//------------------------------------------------------------------------------
Image::~Image()
{
    clear();
}

//------------------------------------------------------------------------------
void Image::create(Vector2u size, PixelFormat format, Color8 defaultColor)
{
    createNoFill(size, format);
    fill(defaultColor);
}

//------------------------------------------------------------------------------
void Image::createNoFill(Vector2u size, PixelFormat format)
{
    SN_ASSERT(format == SN_IMAGE_RGBA32, "Image format not supported yet");
    if (m_size != size || m_pixelData == nullptr || m_pixelFormat != format)
    {
        clear();
        m_pixelFormat = format;
        m_size = size;
        u32 len = getDataLength();
        m_pixelData = new u8[sizeof(u8) * len];
    }
}

//------------------------------------------------------------------------------
void Image::loadFromPixels(Vector2u size, PixelFormat format, const u8 * pixelData)
{
    SN_ASSERT(format == SN_IMAGE_RGBA32, "Image format not supported yet");

    clear();
    m_pixelFormat = format;
    m_size = size;

    u32 len = getDataLength();

    if (len)
    {
        m_pixelData = new u8[sizeof(u8)* len];
        memcpy(m_pixelData, pixelData, len);
    }
}

//------------------------------------------------------------------------------
void Image::clear()
{
    if (m_pixelData)
    {
        delete[] m_pixelData;
        m_pixelData = nullptr;
    }
    m_size = Vector2u(0, 0);
}

//------------------------------------------------------------------------------
bool Image::getPixel(u32 x, u32 y, Color8 & out_color) const
{
    if (x < m_size.x() && y < m_size.y() && m_pixelData)
    {
        const u8 * p = &m_pixelData[getPixelIndex(x, y)];
        out_color.set(p[0], p[1], p[2], p[3]);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool Image::setPixel(u32 x, u32 y, Color8 c)
{
    if (x < m_size.x() && y < m_size.y() && m_pixelData)
    {
        u8 * p = &m_pixelData[getPixelIndex(x, y)];
        p[0] = c.r;
        p[1] = c.g;
        p[2] = c.b;
        p[3] = c.a;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
void Image::fill(Color8 color)
{
    if (m_pixelData)
    {
        u32 pixelCount = m_size.x() * m_size.y();
        if (pixelCount)
        {
            for (u32 i = 0; i < pixelCount; ++i)
            {
                u32 j = i * 4;
                m_pixelData[j++] = color.r;
                m_pixelData[j++] = color.g;
                m_pixelData[j++] = color.b;
                m_pixelData[j++] = color.a;
            }
        }
    }
}

//------------------------------------------------------------------------------
void Image::pasteSubImage(const Image & image, s32 x, s32 y)
{
    pasteSubImage(image.getPixelsPtr(), x, y, image.getSize().x(), image.getSize().y(), image.getPixelFormat());
}

//------------------------------------------------------------------------------
void Image::pasteSubImage(const u8 * pixels, s32 x, s32 y, u32 w, u32 h, PixelFormat format)
{
    SN_ASSERT(pixels != nullptr, "Pixels pointer is null");
    SN_ASSERT(format == m_pixelFormat, "Pixel formats don't match");

    if (x > static_cast<s32>(m_size.x()) || 
        y > static_cast<s32>(m_size.y()) ||
        x + static_cast<s32>(w) <= 0 || 
        y + static_cast<s32>(h) <= 0)
    {
        SN_WARNING("Pasting out of bounds sub-image");
        return;
    }

    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if (x + w > m_size.x())
        w = m_size.x() - x;
    if (y + h > m_size.y())
        h = m_size.y() - y;
    
    if (w == 0 || h == 0)
        return;

    // Copy row by row
    for (u32 srcY = 0; srcY < w; ++y)
    {
        size_t src_i = srcY * w * 4;
        size_t dst_i = getPixelIndex(x, y + srcY);
        memcpy(m_pixelData + dst_i, pixels + src_i, w * 4);
    }
}

//------------------------------------------------------------------------------
Image & Image::operator=(const Image & other)
{
    if (m_size != other.m_size)
    {
        createNoFill(other.m_size, other.m_pixelFormat);
    }
    if (m_pixelData)
    {
        memcpy(m_pixelData, other.m_pixelData, sizeof(u8)* getDataLength());
    }
    return *this;
}

} // namespace sn

