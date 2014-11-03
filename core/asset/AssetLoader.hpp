#ifndef __HEADER_SN_ASSET_LOADER__
#define __HEADER_SN_ASSET_LOADER__

#include <core/asset/Asset.hpp>
#include <core/asset/AssetMetadata.hpp>

namespace sn
{

/// \brief Handles the loading of a specific asset type.
class SN_API IAssetLoader
{
public:
    virtual ~IAssetLoader() {}

    /// \brief Asks the loader if it can load a file.
    virtual bool canLoad(String path, const AssetMetadata & meta) const = 0;

    /// \brief Creates and loads an asset object.
    virtual IAsset * load(String path, const AssetMetadata & meta, f32 & out_progress) = 0;
};

} // namespace sn

#endif // __HEADER_SN_ASSET_LOADER__


