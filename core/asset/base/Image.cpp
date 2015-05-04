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

