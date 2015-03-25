#include <core/asset/AssetDatabase.hpp>
//#include <core/asset/base/ImageBase.hpp>

#include "gl_check.hpp"
#include "Texture.hpp"

namespace sn {
namespace render {

Texture::Texture() : Asset(),
    m_textureID(0),
    //m_samplerID(0),
    m_isSmooth(false),
    m_isRepeated(false)
{
}

Texture::~Texture()
{
    destroy();
}

bool Texture::canLoad(const AssetMetadata & meta) const
{
    // TODO
    //return AssetDatabase::get().canLoad<ImageBase>();
    return false;
}

bool Texture::loadFromStream(std::ifstream & ifs)
{
    // TODO
    return false;
}

void Texture::destroy()
{
    if (m_textureID)
    {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

bool Texture::create(Vector2u size)
{
    return loadFromPixelsRGBA8(size, nullptr);
}

bool Texture::loadFromPixelsRGBA8(Vector2u size, const char * data)
{
    // Note: data can be null in the case we don't want to initialize pixels
    SN_ASSERT(size.x() > 0 && size.y() > 0, "Invalid pixel data");

    m_size = size;

    if (m_textureID == 0)
    {
        glCheck(glGenTextures(1, &m_textureID));
        //glGenSamplers(1, &m_samplerID);
    }

    if (m_textureID)
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, m_textureID));

        // Set image data
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x(), size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

        // Set wrapping for X and Y
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));

        // Set upscale and downscale filters
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

        return true;
    }
    else
    {
        SN_ERROR("Couldn't create texture");
        return false;
    }
}

//Texture & Texture::operator=(const Texture & other)
//{
//
//}

void Texture::bind(Texture * tex)
{
    glCheck(glBindTexture(GL_TEXTURE_2D, tex->m_textureID));
}

void Texture::setActive(u32 textureUnit, Texture * tex)
{
    glCheck(glActiveTexture(GL_TEXTURE0 + textureUnit));
    Texture::bind(tex);
}

} // namespace render
} // namespace sn

