#ifndef __HEADER_SNR_TEXTURE__
#define __HEADER_SNR_TEXTURE__

#include <core/asset/Asset.hpp>

namespace sn {
namespace render {

class Texture : public Asset
{
public:
    SN_ASSET(sn::render::Texture)

    Texture();

    bool canLoad(const AssetMetadata & meta) const override;
    bool loadFromStream(std::ifstream & ifs) override;

private:
    ~Texture();

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_TEXTURE__

