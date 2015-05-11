#include "VideoDriver.hpp"
#include "Texture.hpp"

namespace sn {
namespace render {

sn::TextureBase * VideoDriver::createTexture()
{
    return new Texture();
}

} // namespace render
} // namespace sn

