/*
AssetMetadata.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "AssetMetadata.h"
#include <core/sml/SmlParser.h>

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
    SmlParser parser;
    Variant o;
    parser.parseValue(ifs, o);

    if (o.isDictionary())
    {
        variantData = o;
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

