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

    // Delete types
    for (auto it = m_types.begin(); it != m_types.end(); ++it)
    {
        delete it->second;
    }
}

//------------------------------------------------------------------------------
bool AssetDatabase::addType(std::string type, IAssetType * loader)
{
    auto it = m_types.find(type);
    if (it != m_types.end())
    {
        SN_ERROR("Cannot add new loader, the asset type \"" << type << "\" is already registered");
        return false;
    }
    else
    {
        m_types.insert(std::make_pair(type, loader));
        return true;
    }
}

//------------------------------------------------------------------------------
IAssetType * AssetDatabase::getType(std::string type)
{
    auto it = m_types.find(type);
    if (it != m_types.end())
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
//bool AssetDatabase::releaseAsset(IAsset * asset)
//{
//    // TODO AssetDatabase::releaseAsset
//    return false;
//}

//------------------------------------------------------------------------------
// Gets an asset. If it returns null, the asset may not have been loaded or is in progress.
Asset * AssetDatabase::getAsset(const std::string & callingModule, const std::string & type, const String & name)
{
    // TODO AssetDatabase::getAsset
    return nullptr;
}

//------------------------------------------------------------------------------
AssetMetadata * AssetDatabase::getAssetMetadata(Asset * asset)
{
    // TODO AssetDatabase::getAssetMetadata
    return nullptr;
}

} // namespace sn



