/*
AssetDatabase.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSETDATABASE__
#define __HEADER_SN_ASSETDATABASE__

#include <core/util/typecheck.hpp>
#include <core/asset/Asset.hpp>
#include <core/asset/AssetMetadata.hpp>
#include <core/app/ModuleInfo.hpp>
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

	static AssetDatabase & get();

	void setRoot(const String & root);

    /// \brief Loads all assets contained in a given module directory.
    /// This function blocks until everything is loaded.
    void loadAssets(const ModuleInfo & modInfo);

    AssetLoadStatus loadAssetFromFile(const String & path, const std::string & moduleName);
    //bool releaseAsset(IAsset * asset);

    // Gets an asset. If it returns null, the asset may not have been loaded or is in progress.
    Asset * getAsset(const std::string & moduleName, const std::string & type, const std::string & name);

    void releaseAssets();

    //-----------------------------
    // Helpers
    //-----------------------------

    // Template version of getAsset, compiled in your native code.
    // It works only if you used the SN_ASSET macro of your asset class.
    template <class Asset_T>
    Asset_T * getAsset(const std::string & moduleName, const std::string & name)
    {
        // Note: use SN_ASSET in your asset class
        Asset * a = getAsset(moduleName, Asset_T::__sGetClassName(), name);
        if (a)
            return checked_cast<Asset_T*>(a);
        else
            return nullptr;
    }

private:
    // Tests if an asset file can be loaded, and if yes, returns its container object in an empty state,
    // with associated metadata.
    // If you don't need the container, you have to call release() on it.
    // If the asset has already been preloaded, returns the asset directly.
    Asset * preloadAsset(const String & path, const std::string & moduleName);

    AssetLoadStatus loadAsset(Asset * asset);

private:

    // Top directory where assets are located (usually the "projects" directory)
    String m_root;

    // [path][asset]
    std::unordered_map<String, Asset*> m_fileCache;

    // [moduleName][typeName][name] => Asset
    std::unordered_map< std::string, std::unordered_map< std::string, std::unordered_map<std::string, Asset*> > > m_assets;

};

} // namespace sn

#endif // __HEADER_SN_ASSETDATABASE__


