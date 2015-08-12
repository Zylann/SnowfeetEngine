/*
AssetMetadata.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSET_METADATA__
#define __HEADER_SN_ASSET_METADATA__

#include <core/util/Variant.h>
#include <core/util/String.h>

namespace sn
{

enum SN_API AssetLoadingMode
{
    SN_ALM_STATIC, // The asset will be loaded when its module is loaded
    SN_ALM_BACKGROUND, // The asset will be loaded in a background thread
    SN_ALM_LAZY // The asset will be loaded when it is first required
};

class SN_API AssetMetadata
{
public:
    //static const char * fileExtension = ".meta";
    AssetMetadata() :
        loadingMode(SN_ALM_STATIC)
    {}

    bool loadFromFile(String metaPath);
    bool saveToFile(String metaPath);

    bool loadFromStream(std::ifstream & ifs);
    bool saveToStream(std::ofstream & ofs);

    std::string type;
    std::string module;
    String path; // File path
    std::string name;
    AssetLoadingMode loadingMode;
    Variant variantData;
};

} // namespace sn

#endif // __HEADER_SN_ASSET_METADATA__

