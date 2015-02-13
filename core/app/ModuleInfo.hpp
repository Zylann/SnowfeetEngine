/*
ModuleInfo.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

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
    std::vector<String> dependencies;
    String startupScene;
    std::vector<std::string> services; // class names of sticky entities to create on load

    //! The order is important in this container !//
    std::vector<String> bindings;
};

} // namespace sn


#endif // __HEADER_MODULE_INFO__





