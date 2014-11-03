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

#ifdef SN_DEBUG
    SN_DLOG("Parsing \"" << filePath << '"');
#endif

    // Parse stream

    document.loadFromStream(ifs);
    ifs.close();

    // Check document

    assert(document.isObject());

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

} // namespace sn


