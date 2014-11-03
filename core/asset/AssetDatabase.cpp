#include "AssetDatabase.hpp"

namespace sn
{

//------------------------------------------------------------------------------
AssetDatabase::AssetDatabase(String root):
    m_root(root)
{
}

//------------------------------------------------------------------------------
AssetDatabase::~AssetDatabase()
{
    // Delete assets
    // TODO AssetDatabase::~AssetDatabase

    // Delete loaders
    for (auto it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        delete it->second;
    }
}

//------------------------------------------------------------------------------
bool AssetDatabase::addLoader(std::string type, IAssetLoader * loader)
{
    auto it = m_loaders.find(type);
    if (it != m_loaders.end())
    {
        SN_ERROR("Cannot add new loader, the asset type \"" << type << "\" is already registered");
        return false;
    }
    else
    {
        m_loaders.insert(std::make_pair(type, loader));
        return true;
    }
}

//------------------------------------------------------------------------------
IAssetLoader * AssetDatabase::getLoader(std::string type)
{
    auto it = m_loaders.find(type);
    if (it != m_loaders.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
bool AssetDatabase::loadAssetFromFile(String path, bool reload)
{
    // TODO AssetDatabase::loadAssetFromFile
    return false;
}

//------------------------------------------------------------------------------
bool AssetDatabase::releaseAsset(IAsset * asset)
{
    // TODO AssetDatabase::releaseAsset
    return false;
}

//------------------------------------------------------------------------------
// Gets an asset. If it returns null, the asset may not have been loaded or is in progress.
IAsset * AssetDatabase::getAsset(const std::string & callingModule, const std::string & type, const String & name)
{
    // TODO AssetDatabase::getAsset
    return nullptr;
}

//------------------------------------------------------------------------------
AssetMetadata * AssetDatabase::getAssetMetadata(IAsset * asset)
{
    // TODO AssetDatabase::getAssetMetadata
    return nullptr;
}

} // namespace sn



