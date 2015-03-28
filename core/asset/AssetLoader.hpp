#ifndef __HEADER_SN_ASSETLOADER__
#define __HEADER_SN_ASSETLOADER__

#include <core/reflect/Object.hpp>
#include <core/asset/Asset.hpp>

namespace sn
{

/// \brief Classes inheriting this one will be used to load generic asset types.
/// It's an alternative to the inheritance-based system currently in place.
class SN_API AssetLoader : public Object
{
public:
    SN_OBJECT(sn::AssetLoader, sn::Object)

    virtual ~AssetLoader() {}

    /// \brief Returns which kind of asset this loader is designed for.
    virtual const ObjectType & getAssetType() const = 0;

    /// \brief Tests if an asset can be loaded.
    /// The asset identifier is given as available metadata before reading the actual source data.
    /// \return true on success, false on failure.
    virtual bool canLoad(const AssetMetadata & meta) const = 0;

    /// \brief Loads the given asset. If the given instance is already in a loaded state, it is reloaded
    /// (This can be seen as a part of the hot-reloading feature)
    /// \note You have to cast the input to be able to work with your asset type.
    /// To ensure safety it is recommended to check the ObjectType before.
    /// \return true on success, false on failure.
    virtual bool load(std::ifstream & ifs, Asset & asset) const = 0;

};

} // namespace sn

#endif // __HEADER_SN_ASSETLOADER__

