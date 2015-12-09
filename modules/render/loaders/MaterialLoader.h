#ifndef __HEADER_SNR_MATERIALLOADER__
#define __HEADER_SNR_MATERIALLOADER__

#include <core/asset/AssetLoader.h>
#include <core/util/Variant.h>

namespace sn
{

class MaterialLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::MaterialLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

private:
    bool loadFromVariant(const sn::Variant & doc, sn::Material & mat) const;

};

} // namespace sn

#endif // __HEADER_SNR_MATERIALLOADER__

