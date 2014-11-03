#ifndef __HEADER_MODULE_INFO__
#define __HEADER_MODULE_INFO__

#include <core/util/String.hpp>
#include <vector>

namespace sn
{

struct ModuleInfo
{
    bool loadFromFile(const String & pathToProjects, const String & modPath);

    std::string name;
    String directory;
    String modFilePath;
    std::string scriptNamespace;
    std::string mainClass;
    std::vector<String> bindings;
    std::vector<String> dependencies;
};

} // namespace sn


#endif // __HEADER_MODULE_INFO__





