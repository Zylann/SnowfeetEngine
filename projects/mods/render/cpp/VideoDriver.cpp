#include "VideoDriver.h"
#include "Texture.h"

namespace sn {
namespace render {

sn::TextureBase * VideoDriver::createTexture()
{
    return new Texture();
}

} // namespace render
} // namespace sn

