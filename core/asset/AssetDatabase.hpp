/*
AssetDatabase.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSETDATABASE__
#define __HEADER_SN_ASSETDATABASE__

#include <core/util/typecheck.hpp>
#include <core/asset/AssetLoader.hpp>
#include <core/asset/AssetMetadata.hpp>
#include <core/asset/SerializationContext.hpp>
#include <core/app/ModuleInfo.hpp>
#include <core/system/file/FileWatcher.hpp>

#include <unordered_map>


namespace sn
{

enum AssetLoadStatus
{
    SN_ALS_LOADED = 0,
    SN_ALS_MISMATCH,
    SN_ALS_OPEN_ERROR,
    SN_ALS_READ_ERROR
};

typedef std::vector<const AssetLoader*> AssetLoaderList;

/// \brief Contains all resources currently loaded by the application.
class SN_API AssetDatabase : public NonCopyable
{
public:

    // The name of an asset is the name of its file without the extension.
    // In a module, two assets of the same type cannot have the same name.

    // Note: this class could indeed use templates for each asset type.
    // however, due to the dynamic modular nature of the engine, we can't use compile-time
    // type checking in every part.

    AssetDatabase();
    ~AssetDatabase();

    /// \brief Gets the database singleton.
	static AssetDatabase & get();

    /// \brief Sets the main root that will be used for all assets to be loaded.
    /// \warning: should be only assigned once at the moment.
	void setRoot(const String & root);

    /// \brief Registers all asset loader classes in the specified reflected module.
    void addLoadersFromModule(const std::string & moduleName);

    /// \brief Unregisters all asset loader classes from the specified reflected module.
    void releaseLoadersFromModule(const std::string & moduleName = "");

    /// \brief Unregisters all asset loaders.
    void releaseLoaders();

    /// \brief Loads all assets contained in a given module directory.
    /// This function blocks until everything is loaded.
    void loadAssets(const ModuleInfo & modInfo);

    AssetLoadStatus loadAsset(Asset * asset, const AssetMetadata * a_newMetadata = nullptr);

    // Gets an asset. If it returns null, the asset may not be loadable or has not been registered.
    Asset * getAsset(const std::string & moduleName, const ObjectType & type, const std::string & name);
    Asset * getAsset(const AssetLocation & loc, const std::string & typeName);

    /// \brief Releases all assets contained in the database.
    /// \note Ownership is released. If you still strong-reference an asset somewhere else, it won't be destroyed!
    void releaseAssets();

    //-----------------------------
    // Live edition
    //-----------------------------

    void setTrackFileChanges(bool enabled);
    bool isTrackingFileChanges() const;
    void updateFileChanges();

    //-----------------------------
    // Template helpers
    //-----------------------------

    // Template version of getAsset, compiled in your native code.
    // It works only if you used the SN_ASSET macro of your asset class.
    template <class Asset_T>
    Asset_T * getAsset(const std::string & moduleName, const std::string & name)
    {
        // Note: use SN_ASSET in your asset class
        Asset * a = getAsset(moduleName, Asset_T::__sGetObjectType(), name);
        if (a)
            return checked_cast<Asset_T*>(a);
        else
            return nullptr;
    }

    template <class Asset_T>
    Asset_T * getAsset(const AssetLocation & location)
    {
        return getAsset<Asset_T>(location.module, location.name);
    }

    template <class Asset_T>
    AssetLoader * findLoader()
    {
        return findLoader(Asset_T::__sGetObjectType());
    }

#ifdef SN_BUILD_DEBUG
    void printAssetList() const;
#endif

private:
    void addLoader(AssetLoader * loader);

    /// \brief Finds loaders associated to the given asset metadata.
    /// \param meta: asset info such as the file path, name, load parameters...
    /// \return list of loaders sorted by execution order. Can be empty if no loaders are available.
    AssetLoaderList findLoaders(const AssetMetadata & meta) const;

    /// \brief Finds the loader associated to a specific asset type.
    /// \param assetType: C++ type of the asset.
    /// \return loader, or null if no loader is associated.
    AssetLoader * findLoader(const ObjectType & assetType) const;

    /// \brief Sorts a list of loaders by execution order.
    /// \param chain: loaders to sort
    void orderAssetLoaders(AssetLoaderList & chain) const;

    /// \deprecated
    Asset * legacy_createMatchingAssetType(const AssetMetadata & meta) const;

    /// \brief Analyzes a file and creates one or several asset instances it will produce on runtime.
    /// The file will not be loaded into the instances until loadAsset() is called.
    /// \param path: path to the source file
    /// \param moduleName: name of the module this file is associated to
    /// \return Asset instances. Usually one, but depending on the pipeline it can be more.
    std::vector<Asset*> preloadAssetFile(const String & path, const std::string & moduleName);

    /// \brief Loads or reloads all assets liked to a specific source file.
    /// \param Absolute path to the file
    void loadIndexedAssetsByPath(const std::string & path);

    void addToFileCache(Asset & asset);
    void removeFromFileCache(Asset & asset);

private:

    // Note: "baseTypeName" and "instanceTypeName" should not be mixed up here.
    // See AssetLoader for explanation.

    /// \brief Top directory where assets are located (usually the "projects" directory)
    String m_root;

    /// \brief [loaderModule][assetInstanceTypeName] => loader
    std::unordered_map<std::string, std::unordered_map<std::string, AssetLoader*> > m_loaders;

    /// \brief [path] => asset
    std::unordered_map<String, std::vector<Asset*> > m_fileCache;

    /// \brief [moduleName][baseTypeName][name] => asset
    /// \note The moduleName corresponds to the location of the asset, not its type.
    std::unordered_map< std::string, std::unordered_map< std::string, std::unordered_map<std::string, Asset*> > > m_assets;

    /// \brief Listener used to track file changes when live edition is enabled
    FileWatcher m_rootWatcher;

};


//-----------------------------------------------------------------------------
/// \brief Reads a serialized asset reference, looks it up into the database and returns it if found.
///
/// The algorithm searches the asset in the following modules:
/// - The module where the serialization takes place
/// - The module where the class of 'self' is defined (if provided)
/// - If still fails, returns nullptr.
///
/// \param type: type name of the asset
/// \param locationString: string representing the location of the asset (see AssetLocation)
/// \param contextModule: name of the module where the serialization is taking place
/// \param self (optional): object needing to store the asset (typically the caller)
SN_API Asset * getAssetBySerializedLocation(
    const std::string & type, 
    const std::string & locationString, 
    const std::string & contextModule,
    const Object * self = nullptr
);

// Template version
template <class Asset_T>
Asset_T * getAssetBySerializedLocation(const std::string & locationString, const std::string & contextModule, const Object * self=nullptr)
{
    // Note: use SN_ASSET in your asset class
    Asset * a = getAssetBySerializedLocation(Asset_T::__sGetClassName(), locationString, contextModule, self);
    if (a)
        return checked_cast<Asset_T*>(a);
    else
        return nullptr;
}

} // namespace sn

#endif // __HEADER_SN_ASSETDATABASE__


