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
    u32 count = 0;
    // TODO Ignore special folders such as cpp/
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        const FileNode & file = *it;
        if (loadAssetFromFile(file.path, modInfo.name) == SN_ALS_LOADED)
        {
            ++count;
        }
    }
#ifdef SN_BUILD_DEBUG
    SN_LOG("Loaded " << count << " assets from " << modInfo.name << " in " << clock.getElapsedTime().asSeconds() << " seconds");
#endif
}

//------------------------------------------------------------------------------
AssetLoadStatus AssetDatabase::loadAssetFromFile(const String & path, const std::string & moduleName)
{
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
						return SN_ALS_ERROR;
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

    if (asset == nullptr)
    {
        // Cannot load this file type. The file will be ignored.
        return SN_ALS_MISMATCH;
    }

    const std::string & typeName = asset->getObjectType().getName();

    // Check if not already loaded
    std::string assetName = getFileNameWithoutExtension(toString(path));
    if (getAsset(moduleName, typeName, assetName) != nullptr)
    {
        SN_ERROR("Asset " << toString(path) << " is already loaded "
            "under registry [" << moduleName << "][" << typeName << "][" << assetName << "]");
        asset->release();
        return SN_ALS_ERROR;
    }

    SN_DLOG("Loading asset " << toString(path) << "...");

    // Open file stream
    std::ifstream ifs(metadata.path, std::ios::binary | std::ios::in);
    if (!ifs.good())
    {
        SN_ERROR("Cannot open file " << toString(metadata.path) << " for asset of type " << typeName);
        asset->release();
        return SN_ALS_ERROR;
    }

    // Assign metadata
    asset->m_metadata = metadata;

    // Load asset
    if (asset->loadFromStream(ifs))
    {
        // Success
        ifs.close();
        // Store asset
        m_assets[moduleName][typeName][assetName] = asset;
        return SN_ALS_LOADED;
    }
    else
    {
        // Loading failure, shouldn't have happened
        asset->release();
        SN_ERROR("An error occurred when loading asset " << toString(metadata.path) << " of type " << typeName);
        return SN_ALS_ERROR;
    }
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

} // namespace sn



