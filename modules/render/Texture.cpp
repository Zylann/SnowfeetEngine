#include <core/asset/AssetDatabase.h>
//#include <core/asset/base/ImageBase.h>

#include "gl_check.h"
#include "Texture.h"
#include <GL/glew.h>


namespace sn {
namespace render {

//-----------------------------------------------------------------------------
Texture::Texture() : sn::Asset(),
    m_isSmooth(true),
    m_isRepeated(false),
    m_handle(nullptr),
    m_keepSourceInMemory(false)
{
}

//-----------------------------------------------------------------------------
Texture::~Texture()
{
    destroy();
}

//------------------------------------------------------------------------------
bool Texture::uploadToVRAM()
{
    Image * img = m_image.get();
    bool success = false;
    if (img)
    {
        Image * img = m_image.get();
        bool success = false;
        if (img)
            success = loadFromPixelsRGBA8(img->getSize(), reinterpret_cast<const char*>(img->getPixelsPtr()));
        if (success && !isKeepSourceInMemory())
        {
            img->clear();
        }
    }
    return success;
}

//------------------------------------------------------------------------------
bool Texture::downloadFromVRAM()
{
    SN_WARNING("Texture::downloadFromVRAM(): not implemented yet");
    return false;
}

//------------------------------------------------------------------------------
Image * Texture::getImage() const
{
    return m_image.get();
}

//------------------------------------------------------------------------------
void Texture::setSourceImage(Image & image)
{
    m_image.set(&image);
    m_size = image.getSize();
}

//------------------------------------------------------------------------------
Vector2u Texture::getSize()
{
    if ((m_size.x() == 0 || m_size.y() == 0) && !m_image.isNull())
        return m_image.get()->getSize();
    else
        return m_size;
}

//-----------------------------------------------------------------------------
void Texture::destroy()
{
    GLuint textureID = reinterpret_cast<GLuint>(getHandle());
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

    GLuint textureID = reinterpret_cast<GLuint>(getHandle());

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
    SN_ASSERT(tex != nullptr, "Texture::bind(): texture is null");
    GLuint textureID = reinterpret_cast<GLuint>(tex->getHandle());
    glCheck(glBindTexture(GL_TEXTURE_2D, textureID));
}

//-----------------------------------------------------------------------------
void Texture::setActive(u32 textureUnit, Texture * tex)
{
    glCheck(glActiveTexture(GL_TEXTURE0 + textureUnit));
    Texture::bind(tex);
}

} // namespace render
} // namespace sn

