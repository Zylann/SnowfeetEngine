#include <core/util/Log.hpp>
#include <core/util/stringutils.hpp>
#include <core/util/typecheck.hpp>

#include "ImageLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi/stb_image_write.h"

namespace sn
{

const ObjectType & ImageLoader::getBaseAssetType() const
{
    return Image::__sGetObjectType();
}

bool ImageLoader::canLoad(const AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".png" || ext == L".tga" || ext == L".bmp";
}

bool ImageLoader::load(std::ifstream & ifs, Asset & asset) const
{
    sn::Image * image = checked_cast<Image*>(&asset);

    // Load image
    s32 width = 0;
    s32 height = 0;
    s32 channels = 0;

    // TODO [Optimize] I was tired when writing this
    // TODO I was tired too when reading this again
    std::vector<u8> buffer;
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
    {
        return loadFromMemory(*image, &buffer[0], buffer.size());
    }
}

//------------------------------------------------------------------------------
bool ImageLoader::loadFromMemory(sn::Image & image, const u8 * data, u32 dataSize)
{
    image.clear();

    // Check input parameters
    if (data && dataSize)
    {
        // Load image
        s32 width = 0;
        s32 height = 0;
        s32 channels = 0;
        u8 * pixels = stbi_load_from_memory(data,
            static_cast<s32>(dataSize),
            &width, &height, &channels, STBI_rgb_alpha
        );

        if (pixels && width && height)
        {
            // Assign loaded data
            if (channels == 4)
            {
                image.loadFromPixels(Vector2u(width, height), SN_IMAGE_RGBA32, pixels);
            }
            else
            {
                SN_WERROR(L"Unsupported image format (" << image.getAssetMetadata().path << L")");
            }

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
//bool Image::saveToFile(const std::string & filePath, std::string formatExtension)
//{
//    // Make sure the image is not empty
//    if (!m_pixels.empty() && m_size.x() > 0 && m_size.y() > 0)
//    {
//        if (formatExtension.empty())
//        {
//            // Extract the extension
//            formatExtension = filePath.substr(filePath.size() - 3);
//        }
//
//        toLower(formatExtension);
//
//        // Deduce the image type from its extension
//        if (formatExtension == "bmp")
//        {
//            // BMP format
//            if (stbi_write_bmp(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0]))
//                return true;
//        }
//        else if (formatExtension == "tga")
//        {
//            // TGA format
//            if (stbi_write_tga(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0]))
//                return true;
//        }
//        else if (formatExtension == "png")
//        {
//            // PNG format
//            if (stbi_write_png(filePath.c_str(), m_size.x(), m_size.y(), 4, &m_pixels[0], 0))
//                return true;
//        }
//        else// if (formatExtension == "jpg")
//        {
//            SN_ERROR("Exporting to \"" << formatExtension << "\" format is not supported");
//            //// JPG format
//            //if (writeJpg(filename, pixels, size.x, size.y))
//            //    return true;
//        }
//    }
//
//    SN_ERROR("Failed to save image \"" << filePath << "\", no valid data provided");
//    return false;
//}

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

