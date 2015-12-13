#ifndef __HEADER_SN_IMAGE__
#define __HEADER_SN_IMAGE__

#include <core/asset/Asset.h>
#include <core/math/Vector2.h>

#include <modules/image/common.h>

namespace sn
{

enum PixelFormat
{
    SN_IMAGE_RGBA32 = 0
};

/// \brief 2D container for pixel data, stored as 8 bit components.
class SN_IMAGE_API Image : public Asset
{
public:
    SN_OBJECT(sn::Image, sn::Asset)

    /// \brief Constructs an empty image.
    Image();
    /// \brief Constructs an image as a copy of another
    Image(const Image & other);

    /// \brief Sets the size and format of the image and fills it with a uniform color
    /// \param size: size of the image in pixels
    /// \param format: pixel format of the image
    /// \param defaultColor: which color to fill the image with
    void create(
        Vector2u size,
        PixelFormat format=SN_IMAGE_RGBA32, 
        Color8 defaultColor=Color8(255,255,255,255)
    );

    /// \brief Sets the size and format of the image and fills it with given pixel data.
    /// \param size: size of the input image
    /// \param format: pixel format of the input image
    /// \param pixelData: input image pixels as interleaved components
    void loadFromPixels(
        Vector2u size,
        PixelFormat format,
        const u8 * pixelData
    );

    /// \brief Clears the image and resets its size to zero.
    void clear();

    /// \brief Gets the size of the image in pixels
    Vector2i getSize() const { return m_size; }

    /// \brief Gets the pixel format of the image
    PixelFormat getPixelFormat() const { return m_pixelFormat; }

    /// \brief Gets the number of components have pixels.
    /// \note Only RGBA is supported at the moment, so this method always returns 4.
    u32 getChannelCount() const { return 4; }

    /// \brief Gets the color of an individual pixel.
    /// \param x: X coordinate of the pixel
    /// \param y: Y coordinate of the pixel
    /// \param out_color: this parameter will be filled with the pixel color if available.
    /// \return true if the pixel is available, false otherwise.
    bool getPixel(u32 x, u32 y, Color8 & out_color) const;

    /// \brief Sets the color of an individual pixel.
    /// \param x: X coordinate of the pixel
    /// \param y: Y coordinate of the pixel
    /// \param c: color to set
    /// \return true if the pixel is available, false otherwise.
    bool setPixel(u32 x, u32 y, Color8 c);

    /// \brief Gets the raw pixel data of the image.
    /// \return pixel data as interleaved components
    const u8 * getPixelsPtr() const { return m_pixelData; }

    /// \brief Gets the raw index of a pixel.
    u32 getPixelIndex(u32 x, u32 y) const { return (x + m_size.x() * y) * getChannelCount(); }

    /// \brief Fills the image with a color.
    void fill(Color8 color);

    /// \brief Copies the pixels from an image into this image.
    void pasteSubImage(const Image & image, s32 x, s32 y);
    /// \brief Copies raw pixels into a sub-rectangle of the image.
    void pasteSubImage(const u8 * pixels, s32 x, s32 y, u32 w, u32 h, PixelFormat format);

    /// \brief Copies an image.
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

