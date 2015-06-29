/*
AssetMetadata.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "AssetMetadata.h"

namespace sn
{

//------------------------------------------------------------------------------
bool AssetMetadata::loadFromFile(String metaPath)
{
    metaPath += L".meta";
    std::ifstream ifs(metaPath.c_str(), std::ios::in|std::ios::binary);
    if (ifs.good())
        return loadFromStream(ifs);
    return false;
}

//------------------------------------------------------------------------------
bool AssetMetadata::saveToFile(String metaPath)
{
    SN_WARNING("AssetMetadata::saveToFile() is not implemented yet");
    return false;
}

//------------------------------------------------------------------------------
bool AssetMetadata::loadFromStream(std::ifstream & ifs)
{
    JsonBox::Value o;
    o.loadFromStream(ifs);

    if (o.isObject())
    {
        json = o;
        return true;
    }
    else
    {
        SN_ERROR("Expected JSON object in meta file");
        return false;
    }
}

//------------------------------------------------------------------------------
bool AssetMetadata::saveToStream(std::ofstream & ofs)
{
    SN_WARNING("AssetMetadata::saveToStream() is not implemented yet");
    return false;
}

} // namespace sn

