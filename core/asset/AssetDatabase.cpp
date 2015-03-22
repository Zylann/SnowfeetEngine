/*
AssetDatabase.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "AssetDatabase.hpp"
#include <core/system/file/filesystem.hpp>
#include <core/reflect/ObjectTypeDatabase.hpp>
#include <core/util/Log.hpp>
#include <core/util/stringutils.hpp>

#ifdef SN_BUILD_DEBUG
#include <core/system/time/Clock.hpp>
#endif

namespace sn
{

//------------------------------------------------------------------------------
AssetDatabase & AssetDatabase::get()
{
	static AssetDatabase m_database;
	return m_database;
}

//------------------------------------------------------------------------------
AssetDatabase::AssetDatabase()
{
}

//------------------------------------------------------------------------------
void AssetDatabase::setRoot(const String & root)
{
	SN_ASSERT(m_assets.empty(), "Cannot set root after assets have been loaded!");
	m_root = root;
}

//------------------------------------------------------------------------------
AssetDatabase::~AssetDatabase()
{
    releaseAssets();
}

//------------------------------------------------------------------------------
void AssetDatabase::loadAssets(const ModuleInfo & modInfo)
{
#ifdef SN_BUILD_DEBUG
    Clock clock;
#endif
    std::vector<FileNode> files;
    getFilesRecursively(FilePath::join(m_root, modInfo.directory), files);

    u32 indexedCount = 0;
    u32 loadedCount = 0;

	std::vector<Asset*> moduleAssets;
    
    // First, index all the files we can load
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        // TODO Ignore special folders such as cpp/
        const FileNode & file = *it;
		Asset * asset = preloadAsset(file.path, modInfo.name);
        if (asset)
        {
			moduleAssets.push_back(asset);
            ++indexedCount;
        }
    }
#ifdef SN_BUILD_DEBUG
    SN_LOG("Indexed " << indexedCount << " assets from " << modInfo.name << " in " << clock.restart().asSeconds() << " seconds");
#endif

    for (auto it = moduleAssets.begin(); it != moduleAssets.end(); ++it)
    {
        Asset * asset = *it;
        loadAsset(asset);
    }
#ifdef SN_BUILD_DEBUG
    SN_LOG("Loaded " << loadedCount << " assets from " << modInfo.name << " in " << clock.getElapsedTime().asSeconds() << " seconds");
#endif
}

//------------------------------------------------------------------------------
Asset * AssetDatabase::preloadAsset(const String & path, const std::string & moduleName)
{
    // Check if not already loaded
    auto assetIt = m_fileCache.find(path);
    if (assetIt != m_fileCache.end())
    {
        // Already loaded, return the asset
        return assetIt->second;
    }

    // Retrieve metadata
    AssetMetadata metadata;
    metadata.path = path;
    metadata.module = moduleName;
    metadata.loadFromFile(path); // Not fatal if the .meta file isn't found

    // Find loader:
    // TODO [Optimize] the following is hammerish and sometimes conflictish. Find a better solution? I'm sure there is one.

    Asset * asset = nullptr;

    const ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    const ObjectTypeMap & types = otb.getTypes();
    const ObjectType & assetType = Asset::__sGetObjectType();

    // Iterate over all classes inheriting sn::Asset and call their canLoad() method
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & t = *(it->second);
        if (t.is(assetType))
        {
            if (!t.isAbstract())
            {
                Asset * candidateAsset = (Asset*)(t.instantiate());
                if (candidateAsset->canLoad(metadata))
                {
                    if (asset == nullptr)
                    {
                        asset = candidateAsset;
                    }
                    else
                    {
                        SN_WERROR(L"Cannot determine which asset loader to use for file '" << path << L"'.");
                        SN_ERROR("Candidates are: " << asset->getObjectType().getName() << ", " << candidateAsset->getObjectType().getName());
                        asset->release();
                        candidateAsset->release();
                        return nullptr;
                    }
                }
                else
                {
                    candidateAsset->release();
                }
            }
            //else
            //{
            //	SN_DLOG("Ignored asset type " << t.toString() << " because it is abstract");
            //}
        }
    }

    if (asset)
    {
        metadata.name = getFileNameWithoutExtension(toString(path));

        // Assign metadata
        asset->m_metadata = metadata;

        // Store in file mapping
        m_fileCache[metadata.path] = asset;

        // Store in asset mapping
        m_assets[metadata.module][asset->getObjectType().getName()][metadata.name] = asset;

        SN_DLOG("Preloaded asset " << toString(path) << "...");
    }

    // If nullptr, the asset wasn't loaded and couldn't be loaded
    return asset;
}

//------------------------------------------------------------------------------
AssetLoadStatus AssetDatabase::loadAsset(Asset * asset)
{
    SN_ASSERT(asset != nullptr, "Cannot receive nullptr");

    const AssetMetadata & metadata = asset->getAssetMetadata();
    const std::string & typeName = asset->getObjectType().getName();

    SN_DLOG("Loading asset " << toString(metadata.path) << "...");

    // Open file stream
    std::ifstream ifs(metadata.path, std::ios::binary | std::ios::in);
    if (!ifs.good())
    {
        SN_ERROR("Cannot open file " << toString(metadata.path) << " for asset of type " << typeName);
        //asset->release();
        return SN_ALS_OPEN_ERROR;
    }

    // Load asset
    if (asset->loadFromStream(ifs))
    {
        // Success
        ifs.close();
        // Store asset
        //m_assets[moduleName][typeName][assetName] = asset;
        return SN_ALS_LOADED;
    }
    else
    {
        // Loading failure
        //asset->release();
        SN_ERROR("An error occurred when loading asset " << toString(metadata.path) << " of type " << typeName);
        return SN_ALS_READ_ERROR;
    }
}

//------------------------------------------------------------------------------
AssetLoadStatus AssetDatabase::loadAssetFromFile(const String & path, const std::string & moduleName)
{
    Asset * asset = preloadAsset(path, moduleName);

    const AssetMetadata & metadata = asset->getAssetMetadata();
#ifdef SN_BUILD_DEBUG
    SN_ASSERT(metadata.path == path, "Invalid state");
#endif

    if (asset == nullptr)
    {
        // Cannot load this file type. The file will be ignored.
        return SN_ALS_MISMATCH;
    }

    return loadAsset(asset);
}

//------------------------------------------------------------------------------
//bool AssetDatabase::releaseAsset(IAsset * asset)
//{
//    // TODO AssetDatabase::releaseAsset
//    return false;
//}

//------------------------------------------------------------------------------
void AssetDatabase::releaseAssets()
{
    u32 leakCount = 0;
    for (auto it1 = m_assets.begin(); it1 != m_assets.end(); ++it1)
    {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
        {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
            {
                Asset * asset = it3->second;
#ifdef SN_BUILD_DEBUG
                AssetMetadata meta = asset->getAssetMetadata();
#endif
                u32 refcount = asset->release();
                if (refcount > 0)
                {
#ifdef SN_BUILD_DEBUG
                    SN_WWARNING(L"Asset " << meta.path << L" is still referenced " << refcount << L" times");
#endif
                    ++leakCount;
                }
            }
        }
    }
    if (leakCount > 0)
    {
        SN_WARNING(leakCount << " assets are still referenced after deletion from the database");
    }
    m_assets.clear();
    m_fileCache.clear();
}

//------------------------------------------------------------------------------
// Gets an asset. If it returns null, the asset may not have been loaded or is in progress.
Asset * AssetDatabase::getAsset(const std::string & moduleName, const std::string & type, const std::string & name)
{
    auto it1 = m_assets.find(moduleName);
    if (it1 != m_assets.end())
    {
        auto it2 = it1->second.find(type);
        if (it2 != it1->second.end())
        {
            auto it3 = it2->second.find(name);
            if (it3 != it2->second.end())
                return it3->second;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Asset * AssetDatabase::getAsset(const AssetLocation & loc, const std::string & type)
{
    return getAsset(loc.module, type, loc.name);
}

//------------------------------------------------------------------------------
Asset * getAssetBySerializedLocation(const std::string & type, const std::string & locationString, const std::string & contextModule, Object * self)
{
    AssetLocation location(locationString);
    if (!location.isEmpty())
    {
        AssetDatabase & adb = AssetDatabase::get();

        if (!location.module.empty())
        {
            // The module is specified, no need for lookup
            return adb.getAsset(location, type);
        }

        // No module specified, begin lookup series

        // Try to get the asset from the module the serialization is occurring into
        location.module = contextModule;
        Asset * asset = adb.getAsset(location, type);
        if (asset)
            return asset;

        if (self)
        {
            // Try from the module where the class of 'self' is defined
            location.module = self->getObjectType().getModuleName();
            asset = adb.getAsset(location, type);
            if (asset)
                return asset;
        }
    }

    // Asset not found
    return nullptr;
}

} // namespace sn



