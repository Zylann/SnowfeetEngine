#ifndef __HEADER_SNR_BASICMESHLOADER__
#define __HEADER_SNR_BASICMESHLOADER__

#include <core/asset/AssetLoader.h>

namespace sn
{

class BasicMeshLoader : public AssetLoader
{
public:
    SN_OBJECT

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

};

} // namespace sn

#endif // __HEADER_SNR_BASICMESHLOADER__

