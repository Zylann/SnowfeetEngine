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
    return m_size;
}

} // namespace sn

