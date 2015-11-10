#include <core/asset/AssetDatabase.h>
//#include <core/asset/base/ImageBase.h>

#include "gl_check.h"
#include "Texture.h"
#include <GL/glew.h>


namespace sn {
namespace render {

//-----------------------------------------------------------------------------
Texture::Texture() : TextureBase(),
    m_isSmooth(true),
    m_isRepeated(false)
{
}

//-----------------------------------------------------------------------------
Texture::~Texture()
{
    destroy();
}

//-----------------------------------------------------------------------------
bool Texture::onUploadToVRAM()
{
    Image * img = m_image.get();
    bool success = false;
    if (img)
        success = loadFromPixelsRGBA8(img->getSize(), reinterpret_cast<const char*>(img->getPixelsPtr()));
    return success;
}

//-----------------------------------------------------------------------------
bool Texture::onDownloadFromVRAM()
{
    SN_WARNING("Texture::downloadFromVRAM(): not implemented yet");
    return false;
}

//-----------------------------------------------------------------------------
void Texture::destroy()
{
    GLuint textureID = getInternalID();
    if (textureID)
    {
        glDeleteTextures(1, &textureID);
        m_handle = 0;
    }
}

//-----------------------------------------------------------------------------
bool Texture::create(Vector2u size)
{
    return loadFromPixelsRGBA8(size, nullptr);
}

//-----------------------------------------------------------------------------
bool Texture::loadFromPixelsRGBA8(Vector2u size, const char * data)
{
    // Note: data can be null in the case we don't want to initialize pixels
    SN_ASSERT(size.x() > 0 && size.y() > 0, "Invalid pixel data");

    m_size = size;

    GLuint textureID = getInternalID();

    if (textureID == 0)
    {
        glCheck(glGenTextures(1, &textureID));
        m_handle = reinterpret_cast<TextureHandle>(textureID);
        //glGenSamplers(1, &m_samplerID);
    }

    if (textureID)
    {
        bind(this);

        // Set image data
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x(), size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

        updateRepeat();
        updateFilter();

        return true;
    }
    else
    {
        SN_ERROR("Couldn't create texture");
        return false;
    }
}

//-----------------------------------------------------------------------------
void Texture::setSmooth(bool enable)
{
    m_isSmooth = enable;
    if (m_handle)
    {
        bind(this);
        updateFilter();
    }
}

//-----------------------------------------------------------------------------
void Texture::setRepeated(bool enable)
{
    m_isRepeated = enable;
    if (m_handle)
    {
        bind(this);
        updateRepeat();
    }
}

//-----------------------------------------------------------------------------
void Texture::updateFilter()
{
    // Set upscale and downscale filters
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
}

void Texture::updateRepeat()
{
    // Set wrapping for X and Y
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
}

//-----------------------------------------------------------------------------
//Texture & Texture::operator=(const Texture & other)
//{
//
//}

//-----------------------------------------------------------------------------
void Texture::bind(Texture * tex)
{
    glCheck(glBindTexture(GL_TEXTURE_2D, tex->getInternalID()));
}

//-----------------------------------------------------------------------------
void Texture::setActive(u32 textureUnit, Texture * tex)
{
    glCheck(glActiveTexture(GL_TEXTURE0 + textureUnit));
    Texture::bind(tex);
}

} // namespace render
} // namespace sn

