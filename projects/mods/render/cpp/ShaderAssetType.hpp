#ifndef __HEADER_SN_SHADER_ASSET_TYPE__
#define __HEADER_SN_SHADER_ASSET_TYPE__

#include <core/asset/AssetType.hpp>
#include <core/asset/AssetMetadata.hpp>

namespace sn
{

class SN_API ShaderAssetType : public IAssetType
{
public:
    bool canLoad(String path, const AssetMetadata & meta) const override;
    Asset * createInstance() override;
};

} // namespace sn

#endif // __HEADER_SN_SHADER_ASSET_TYPE__

