#ifndef __HEADER_RENDER_SHADER_ASSET_TYPE__
#define __HEADER_RENDER_SHADER_ASSET_TYPE__

#include <core/asset/AssetType.hpp>
#include <core/asset/AssetMetadata.hpp>

namespace sn {
namespace render {

class SN_API ShaderAssetType : public IAssetType
{
public:
    bool canLoad(String path, const AssetMetadata & meta) const override;
    Asset * createInstance() override;
};

} // namespace render
} // namespace sn

#endif // __HEADER_RENDER_SHADER_ASSET_TYPE__

