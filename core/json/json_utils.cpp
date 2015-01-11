#include <fstream>
#include <core/json/json_utils.hpp>
#include <core/util/Log.hpp>

namespace sn
{

//------------------------------------------------------------------------------
bool loadFromFile(JsonBox::Value & document, const std::string & filePath, s32 checkVersion, bool openError)
{
    std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
    if(!ifs.good())
    {
        if(openError)
        {
            SN_ERROR("loadFromFile: couldn't open JSON file \"" + filePath + '"');
        }
        return false;
    }

#ifdef SN_BUILD_DEBUG
    SN_DLOG("Parsing \"" << filePath << '"');
#endif

    // Parse stream

    document.loadFromStream(ifs);
    ifs.close();

    // Check document

    SN_ASSERT(document.isObject(), "JSON document must be an object");

    if(checkVersion >= 0)
    {
        s32 version = document["version"].getInt();
        if(version != checkVersion)
        {
            SN_ERROR("loadFromFile: JSON not compatible "
                "(version=" << version << ", expected " << version << ") "
                "into file \"" << filePath << ". "
                "Maybe this file is from another version of the program?");
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
bool saveToFile(JsonBox::Value & document, const std::string & filePath)
{
    std::ofstream ofs(filePath.c_str(), std::ios::out | std::ios::binary);
    if (!ofs.good())
    {
        SN_ERROR("saveToFile: coudl'nt open JSON file \"" + filePath + '"');
    }
    return false;

#ifdef SN_BUILD_DEBUG
    SN_DLOG("Saving \"" << filePath << '"');
#endif

    SN_ASSERT(document.isObject(), "JSON document must be an object");
    document.writeToStream(ofs);
    ofs.close();
}

} // namespace sn


