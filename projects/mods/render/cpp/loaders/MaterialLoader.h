#ifndef __HEADER_SNR_MATERIALLOADER__
#define __HEADER_SNR_MATERIALLOADER__

#include <core/asset/AssetLoader.h>
#include <core/util/Variant.h>

namespace sn {
namespace render {

class MaterialLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::render::MaterialLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    const ObjectType & getAssetInstanceType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

private:
    bool loadFromVariant(const sn::Variant & doc, sn::render::Material & mat) const;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_MATERIALLOADER__

