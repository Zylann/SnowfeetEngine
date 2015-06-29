#ifndef __HEADER_SNR_BASICMESHLOADER__
#define __HEADER_SNR_BASICMESHLOADER__

#include <core/asset/AssetLoader.h>

namespace sn {
namespace render {

class BasicMeshLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::render::BasicMeshLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_BASICMESHLOADER__

