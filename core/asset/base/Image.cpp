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
    SN_ASSERT(format == SN_IMAGE_RGBA32, "Image format not supported yet");
    clear();
    m_pixelFormat = format;
    m_size = size;
    fill(defaultColor);
}

//------------------------------------------------------------------------------
void Image::loadFromPixels(Vector2u size, PixelFormat format, const u8 * pixelData)
{
    SN_ASSERT(format == SN_IMAGE_RGBA32, "Image format not supported yet");

    clear();
    m_pixelFormat = format;
    m_size = size;

    u32 channelCount = 4; // R,G,B,A
    u32 len = size.x() * size.y() * channelCount;

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
        u32 len = m_size.x() * m_size.y();
        if (len)
        {
            m_pixelData = new u8[sizeof(u8)* 4];
            u32 len4 = len / 4;
            for (u32 i = 0; i < len4; ++i)
            {
                u32 j = i * 4;
                m_pixelData[j  ] = color.r;
                m_pixelData[j+1] = color.g;
                m_pixelData[j+2] = color.b;
                m_pixelData[j+3] = color.a;
            }
        }
    }
}


} // namespace sn

