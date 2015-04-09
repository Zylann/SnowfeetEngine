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
    m_rootWatcher.setFilterDuplicateEvents(true);
}

//------------------------------------------------------------------------------
AssetDatabase::~AssetDatabase()
{
    releaseAssets();
    releaseLoaders();
}

//------------------------------------------------------------------------------
void AssetDatabase::setRoot(const String & root)
{
    SN_ASSERT(m_assets.empty(), "Cannot set root after assets have been loaded!");
    m_root = root;
    m_rootWatcher.setPath(sn::toString(m_root));
}

//------------------------------------------------------------------------------
void AssetDatabase::addLoadersFromModule(const std::string & moduleName)
{
    const ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    const ObjectTypeMap & types = otb.getTypes();
    const ObjectType & loaderType = AssetLoader::__sGetObjectType();

    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & ot = *(it->second);
        if (ot.getModuleName() == moduleName && ot.derivesFrom(loaderType) && !ot.isAbstract())
        {
            AssetLoader * loader = checked_cast<AssetLoader*>(ot.instantiate());
            addLoader(loader);
        }
    }
}

//------------------------------------------------------------------------------
void AssetDatabase::addLoader(AssetLoader * loader)
{
    SN_ASSERT(loader != nullptr, "addLoader() received null pointer");
    SN_ASSERT(loader->getAssetInstanceType().is(loader->getBaseAssetType(), true), "AssetLoader asset types are not compatible!");

    const ObjectType & ot = loader->getObjectType();
    auto & loaders = m_loaders[ot.getModuleName()];

    const ObjectType & assetType = loader->getAssetInstanceType();
    SN_ASSERT(loaders.find(assetType.getName()) == loaders.end(), "Loader registered twice for the same base type!");
    loaders.insert(std::make_pair(assetType.getName(), loader));
}

//------------------------------------------------------------------------------
void AssetDatabase::releaseLoadersFromModule(const std::string & moduleName)
{
    auto modIt = m_loaders.find(moduleName);
    if (modIt != m_loaders.end())
    {
        auto & loaders = modIt->second;
        for (auto it = loaders.begin(); it != loaders.end(); ++it)
        {
            AssetLoader * loader = it->second;
            SN_ASSERT(loader->getObjectType().getModuleName() == moduleName, "Invalid state");
            delete loader;
        }
    }
}

//------------------------------------------------------------------------------
void AssetDatabase::releaseLoaders()
{
    for (auto modIt = m_loaders.begin(); modIt != m_loaders.end(); ++modIt)
    {
        auto & loaders = modIt->second;
        for (auto it = loaders.begin(); it != loaders.end(); ++it)
        {
            AssetLoader * loader = it->second;
            delete loader;
        }
    }
    m_loaders.clear();
}

//------------------------------------------------------------------------------
const AssetLoader * AssetDatabase::findLoader(const AssetMetadata & meta) const
{
    for (auto modIt = m_loaders.begin(); modIt != m_loaders.end(); ++modIt)
    {
        auto & loaders = modIt->second;
        for (auto it = loaders.begin(); it != loaders.end(); ++it)
        {
            AssetLoader & loader = *it->second;
            if (loader.canLoad(meta))
            {
                return &loader;
            }
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
const AssetLoader * AssetDatabase::findLoader(const Asset & asset) const
{
    for (auto modIt = m_loaders.begin(); modIt != m_loaders.end(); ++modIt)
    {
        auto & loaders = modIt->second;

        const ObjectType & assetType = asset.getObjectType();

        auto it = loaders.find(assetType.getName());
        if (it != loaders.end())
        {
            return it->second;
        }

    }
    return nullptr;
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

    Asset * asset = nullptr;

    // Retrieve metadata
    AssetMetadata metadata;
    metadata.path = path;
    metadata.module = moduleName;
    metadata.loadFromFile(path); // Not fatal if the .meta file isn't found

    // Find loader
    const AssetLoader * loader = findLoader(metadata);

    if (loader)
    {
        // Found a loader, the asset is preloaded
        const ObjectType & ot = loader->getAssetInstanceType();
        asset = checked_cast<Asset*>(ot.instantiate());
    }
    else
    {
        // No loader found, use old method
        asset = legacy_createMatchingAssetType(metadata);
    }

    if (asset)
    {
        metadata.name = getFileNameWithoutExtension(toString(path));

        // Assign metadata
        asset->m_metadata = metadata;

        // Store in file mapping
        m_fileCache[metadata.path] = asset;

        // Store in asset mapping using the ObjectType specified by the loader
        const ObjectType & baseObjectType = loader ? loader->getBaseAssetType() : asset->getObjectType();
        m_assets[metadata.module][baseObjectType.getName()][metadata.name] = asset;

        SN_DLOG("Indexed asset " << toString(path) << "...");
    }

    // If nullptr, the asset cannot be loaded
    return asset;
}

//------------------------------------------------------------------------------
Asset * AssetDatabase::legacy_createMatchingAssetType(const AssetMetadata & meta) const
{
    // The following is hammerish and sometimes conflictish.
    // That's why it is currently deprecated, loaders should be used instead.

    Asset * asset = nullptr;

    const ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    const ObjectTypeMap & types = otb.getTypes();
    const ObjectType & assetType = Asset::__sGetObjectType();

    // Iterate over all classes inheriting sn::Asset and call their canLoad() method
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & t = *(it->second);
        if (t.derivesFrom(assetType))
        {
            if (!t.isAbstract())
            {
                Asset * candidateAsset = (Asset*)(t.instantiate());
                if (candidateAsset->canLoad(meta))
                {
                    if (asset == nullptr)
                    {
                        asset = candidateAsset;
                    }
                    else
                    {
                        SN_WERROR(L"Cannot determine which asset loader to use for file '" << meta.path << L"'.");
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

    const AssetLoader * loader = findLoader(*asset);

    if (loader)
    {
        loader->load(ifs, *asset);
        ifs.close();
        return SN_ALS_LOADED;
    }
    else if (asset->loadFromStream(ifs))
    {
        ifs.close();
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
AssetLoadStatus AssetDatabase::loadIndexedAssetByPath(const std::string & path)
{
    auto it = m_fileCache.find(toWideString(path));
    if (it != m_fileCache.end())
    {
        return loadAsset(it->second);
    }
    else
    {
        return SN_ALS_MISMATCH;
    }
}

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
                u32 refcount = asset->release();
                if (refcount > 0)
                {
#ifdef SN_BUILD_DEBUG
                    // Note: sometimes, false-positive leaks are detected, because sometimes,
                    // assets reference others through strong refs. It should be okay, but
                    // the warning message is not valid.
                    AssetMetadata meta = asset->getAssetMetadata();
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
Asset * AssetDatabase::getAsset(const std::string & moduleName, const ObjectType & type, const std::string & name)
{
    auto it1 = m_assets.find(moduleName);
    if (it1 != m_assets.end())
    {
        // Search the specified type
        auto it2 = it1->second.find(type.getName());

        // If not found, search for base types
        const ObjectType * searchType = &type;
        while (it2 == it1->second.end())
        {
            searchType = searchType->getBase();
            if (searchType)
            {
                it2 = it1->second.find(searchType->getName());
            }
            else
            {
                // No more base types and nothing found
                return nullptr;
                // TODO Stop at Asset?
            }
        }

        // Find the asset by name in the container
        auto it3 = it2->second.find(name);
        if (it3 != it2->second.end())
        {
            // Found
            return it3->second;
        }
    }

    // Not found
    return nullptr;
}

//------------------------------------------------------------------------------
Asset * AssetDatabase::getAsset(const AssetLocation & loc, const std::string & typeName)
{
    const ObjectType * ot = ObjectTypeDatabase::get().getType(typeName);
    if (ot)
        return getAsset(loc.module, *ot, loc.name);
    else
        return nullptr;
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

//------------------------------------------------------------------------------
void AssetDatabase::setTrackFileChanges(bool enabled)
{
    //m_rootWatcher.setPath(sn::toString(m_root));
    if (enabled)
        m_rootWatcher.setRecursive(true);
    m_rootWatcher.setEnabled(enabled);
}

//------------------------------------------------------------------------------
bool AssetDatabase::isTrackingFileChanges() const
{
    return m_rootWatcher.isEnabled();
}

//------------------------------------------------------------------------------
void AssetDatabase::updateFileChanges()
{
    if (m_rootWatcher.isEnabled())
    {
        // TODO What if an asset saves itself upon reloading? Infinite loop?
        FileWatcher::Event event;
        while (m_rootWatcher.popEvent(event))
        {
            switch (event.type)
            {
            case FileWatcher::FILE_MODIFIED:
                // TODO win32: two events are received when the asset is saved (CTRL+S). Need to filter the second one!
                SN_DLOG("Received file change: " << event.path);
                loadIndexedAssetByPath(FilePath::join(toString(m_root), event.path));
                break;

            default:
                break;
            }
        }
    }
}

} // namespace sn



