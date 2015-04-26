#include "TextureBase.hpp"

namespace sn
{

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

