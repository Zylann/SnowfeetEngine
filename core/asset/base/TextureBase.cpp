#include "TextureBase.h"

namespace sn
{

//------------------------------------------------------------------------------
bool TextureBase::uploadToVRAM()
{
    Image * img = m_image.get();
    bool success = false;
    if (img)
    {
        success = onUploadToVRAM();
        if (success && !isKeepSourceInMemory())
        {
            img->clear();
        }
    }
    return success;
}

//------------------------------------------------------------------------------
bool TextureBase::downloadFromVRAM()
{
    return onDownloadFromVRAM();
}

//------------------------------------------------------------------------------
Image * TextureBase::getImage() const
{
    return m_image.get();
}

//------------------------------------------------------------------------------
void TextureBase::setSourceImage(Image & image)
{
    m_image.set(&image);
    m_size = image.getSize();
}

//------------------------------------------------------------------------------
Vector2u TextureBase::getSize()
{
    if ((m_size.x() == 0 || m_size.y() == 0) && !m_image.isNull())
        return m_image.get()->getSize();
    else
        return m_size;
}

} // namespace sn

