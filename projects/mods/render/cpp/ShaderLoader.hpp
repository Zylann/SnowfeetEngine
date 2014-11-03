#ifndef __HEADER_SN_SHADER_LOADER__
#define __HEADER_SN_SHADER_LOADER__

#include <core/asset/AssetLoader.hpp>
#include <core/asset/AssetMetadata.hpp>

namespace sn
{

class SN_API ShaderLoader : public IAssetLoader
{
public:
    bool canLoad(String path, const AssetMetadata & meta) const;
    IAsset * load(String path, const AssetMetadata & meta, f32 & out_progress);
};

} // namespace sn

#endif // __HEADER_SN_SHADER_LOADER__

