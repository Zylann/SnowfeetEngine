#include "Texture.hpp"

namespace sn {
namespace render {

Texture::Texture() : Asset()
{

}

Texture::~Texture()
{

}

bool Texture::canLoad(const AssetMetadata & meta) const
{
    // TODO
    return false;
}

bool Texture::loadFromStream(std::ifstream & ifs)
{
    // TODO
    return false;
}


} // namespace render
} // namespace sn

