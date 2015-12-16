#ifndef __HEADER_SN_PACKEDENTITYLOADER__
#define __HEADER_SN_PACKEDENTITYLOADER__

#include <core/asset/AssetLoader.h>

namespace sn
{

class PackedEntityLoader : public AssetLoader
{
public:
    SN_OBJECT

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

};

} // namespace sn

#endif // __HEADER_SN_PACKEDENTITYLOADER__


