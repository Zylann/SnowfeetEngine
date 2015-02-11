#include <core/util/Log.hpp>
#include <core/util/stringutils.hpp>

#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi/stb_image_write.h"

namespace sn
{

//------------------------------------------------------------------------------
Image::Image():
    Asset(),
    m_channels(0)
{
}

//------------------------------------------------------------------------------
Image::~Image()
{
}

//------------------------------------------------------------------------------
bool Image::canLoad(const AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".png" || ext == L".tga" || ext == L".bmp";
}

//------------------------------------------------------------------------------
void Image::create(u32 width, u32 height, Color8 fillColor)
{
    clear();
    m_size = Vector2u(width, height);
    m_channels = 4;
    m_pixels.resize(m_channels * width * height);
    fill(fillColor);
}

//------------------------------------------------------------------------------
void Image::clear()
{
    m_pixels.clear();
    m_size = Vector2u();
    m_channels = 0;
}

//------------------------------------------------------------------------------
bool Image::loadFromStream(std::ifstream & ifs)
{
    // Reset image
    clear();

    // Load image
    s32 width = 0;
    s32 height = 0;
    s32 channels = 0;

    // TODO [Optimize] I was tired when writing this
    std::vector<char> buffer;
    while (!ifs.eof())
    {
        buffer.push_back(ifs.get());
    }

    if (buffer.empty())
    {
        SN_ERROR("The image file is empty");
        return false;
    }
    else
        return loadFromMemory(&buffer[0], buffer.size());
}

//------------------------------------------------------------------------------
bool Image::loadFromMemory(const void * data, u32 dataSize)
{
    clear();

    // Check input parameters
    if (data && dataSize)
    {
        // Load image
        s32 width = 0;
        s32 height = 0;
        s32 channels = 0;
        u8 * pixels = stbi_load_from_memory(
            static_cast<const u8*>(data), 
            static_cast<s32>(dataSize), 
            &width, &height, &channels, STBI_rgb_alpha
        );

        if (pixels && width && height)
        {
            // Assign loaded data
            m_size.x() = static_cast<u32>(width);
            m_size.y() = static_cast<u32>(height);
            m_channels = static_cast<u32>(channels);
            m_pixels.resize(m_size.x() * m_size.y() * m_channels);
            memcpy(&m_pixels[0], pixels, m_pixels.size());

            // Free STB data
            stbi_image_free(pixels);

            return true;
        }
        else
        {
            SN_ERROR("Failed to load image from memory: " << stbi_failure_reason());
            return false;
        }
    }
    else
    {
        SN_ERROR("Failed to load image from memory, no data provided");
        return false;
    }
}

//------------------------------------------------------------------------------
bool Image::loadFromPixels(const u8 * pixels, u32 width, u32 height, u32 channels)
{
    clear();

    if (pixels && width && height)
    {
        m_size.x() = width;
        m_size.y() = height;
        m_channels = channels;
        m_pixels.resize(m_size.x() * m_size.y() * m_channels);
        memcpy(&m_pixels[0], pixels, m_pixels.size());
        return true;
    }
    else
    {
        SN_ERROR("Failed to load image from pixels, no data provided");
        return false;
    }
}

//------------------------------------------------------------------------------
bool Image::saveToFile(const std::string & filePath, std::string formatExtension)
{
    // Make sure the image is not empty
    if (!m_pixels.empty() && m_size.x() > 0 && m_size.y() > 0)
    {
        if (formatExtension.empty())
        {
            // Extract the extension
            formatExtension = filePath.substr(filePath.size() - 3);
        }

        toLower(formatExtension);

        // Deduce the image type from its extension
        if (formatExtension == "bmp")
        {
            // BMP format
            if (stbi_write_bmp(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0]))
                return true;
        }
        else if (formatExtension == "tga")
        {
            // TGA format
            if (stbi_write_tga(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0]))
                return true;
        }
        else if (formatExtension == "png")
        {
            // PNG format
            if (stbi_write_png(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0], 0))
                return true;
        }
        else// if (formatExtension == "jpg")
        {
            SN_ERROR("Exporting to \"" << formatExtension << "\" format is not supported");
            //// JPG format
            //if (writeJpg(filename, pixels, size.x, size.y))
            //    return true;
        }
    }

    SN_ERROR("Failed to save image \"" << filePath << "\", no valid data provided");
    return false;
}

//------------------------------------------------------------------------------
Color8 Image::getPixel(u32 x, u32 y) const
{
    const u8 * ptr = &m_pixels[getPixelIndex(x, y)];
    return Color8(ptr[0], ptr[1], ptr[2], ptr[3]);
}

//------------------------------------------------------------------------------
void Image::setPixel(u32 x, u32 y, Color8 color)
{
    u8 * ptr = &m_pixels[getPixelIndex(x, y)];
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr[3] = color.a;
}

//------------------------------------------------------------------------------
void Image::fill(Color8 color)
{
    unsigned char colorV[4] = { color.r, color.g, color.b, color.a };
    for (unsigned int i = 0; i < m_pixels.size(); i += m_channels)
    {
        memcpy(&m_pixels[i], colorV, m_channels);
    }
}

//------------------------------------------------------------------------------
//void Image::replaceColor(Color8 oldColor, Color8 newColor)
//{
//    unsigned char oldColorV[4] = { oldColor.r, oldColor.g, oldColor.b, oldColor.a };
//    unsigned char newColorV[4] = { newColor.r, newColor.g, newColor.b, newColor.a };
//
//    for (unsigned int i = 0; i < m_pixels.size(); i += m_channels)
//    {
//        unsigned char * pixel = &m_pixels[i];
//        bool match = true;
//        for (unsigned int c = 0; c < m_channels && !match; ++c)
//        {
//            if (pixel[c] != oldColorV[c])
//                match = false;
//        }
//        if (match)
//        {
//            for (unsigned int c = 0; c < m_channels; ++c)
//            {
//                pixel[c] = newColorV[c];
//            }
//        }
//    }
//}

} // namespace sn

