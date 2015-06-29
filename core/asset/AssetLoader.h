#ifndef __HEADER_SN_ASSETLOADER__
#define __HEADER_SN_ASSETLOADER__

#include <core/reflect/Object.h>
#include <core/asset/Asset.h>

namespace sn
{

/// \brief Classes inheriting this one will be used to load generic asset types.
/// It's an alternative to the old deprecated inheritance-based system.
class SN_API AssetLoader : public Object
{
public:
    SN_OBJECT(sn::AssetLoader, sn::Object)

    virtual ~AssetLoader() {}

    /// \brief Returns which kind of asset this loader is designed for in general.
    /// This type will be used as a key in the database, so it is recommended to use
    /// a base asset type defined in the core if possible.
    virtual const ObjectType & getBaseAssetType() const = 0;

    /// \brief Returns the type of asset that will actually be instantiated.
    /// The returned type must be equal or derived rom the one returned by getKeyAssetType().
    /// By default, the value returned is the same as getBaseAssetType().
    virtual const ObjectType & getAssetInstanceType() const { return getBaseAssetType(); }

    /// \brief Tests if an asset can be loaded.
    /// The asset identifier is given as available metadata before reading the actual source data.
    virtual bool canLoad(const AssetMetadata & meta) const = 0;

    /// \brief Tests if an asset should directly be loaded with this loader.
    /// It is useful when the loader internally relies on another (this other one will be indirect).
    /// \note The implementation of this function generally relies on testing the "usage" metadata property.
    virtual bool isDirect(const AssetMetadata & meta) const { return true; }

    /// \brief Loads the given asset. If the given instance is already in a loaded state, it is reloaded
    /// (This can be seen as a part of the hot-reloading feature)
    /// \note You have to cast the input to be able to work with your asset type.
    /// To ensure safety it is recommended to check the ObjectType before.
    /// \return true on success, false on failure.
    virtual bool load(std::ifstream & ifs, Asset & asset) const = 0;

    /// \brief When two loaders match the same files, this function will be called to determine which one runs first.
    /// \param other: loader to compare against
    /// \return -1 = before, 1 = after, 0 = don't care
    virtual s32 getPriority(const AssetLoader & other) const { return 0; }

};

} // namespace sn

#endif // __HEADER_SN_ASSETLOADER__

