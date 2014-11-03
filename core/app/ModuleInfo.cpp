#include "ModuleInfo.hpp"
#include "../json/json_utils.hpp"
#include "../util/stringutils.hpp"
#include <fstream>

namespace sn
{

//------------------------------------------------------------------------------
bool ModuleInfo::loadFromFile(const String & pathToProjects, const String & modPath)
{
    // Reset info
    modFilePath = L"";
    mainClass = "";
    dependencies.clear();
    bindings.clear();
    directory = L"";

    modFilePath = modPath;
    directory = getFileFolder(modFilePath);
    name = fileNameWithoutExtension(toString(modFilePath));

    // Get full path
    String fullPath = pathToProjects + L'/' + modPath;

    // Parse JSON module
    JsonBox::Value v;
    std::ifstream ifs(toString(fullPath), std::ios::in | std::ios::binary);
    if (!ifs.good())
    {
        return false;
    }
    v.loadFromStream(ifs);
    ifs.close();

    // Get main class
    mainClass = v["mainClass"].getString();

    // Get namespace
    scriptNamespace = v["namespace"].getString();

    // Get bindings
    {
        const JsonBox::Array & a = v["bindings"].getArray();
        if (a.size() > 0)
        {
            for (u32 i = 0; i < a.size(); ++i)
            {
                std::string bindingName = a[i].getString();
                bindings.push_back(toWideString(bindingName));
            }
        }
    }

    // Get dependencies
    {
        const JsonBox::Array & a = v["dependencies"].getArray();
        if (a.size() > 0)
        {
            for (u32 i = 0; i < a.size(); ++i)
            {
                std::string bindingName = a[i].getString();
                dependencies.push_back(toWideString(bindingName));
            }
        }
    }

    return true;
}

} // namespace sn

