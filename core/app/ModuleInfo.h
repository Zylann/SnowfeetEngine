/*
ModuleInfo.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_MODULE_INFO__
#define __HEADER_MODULE_INFO__

#include <core/util/String.h>
#include <vector>
#include <core/json/json_utils.h>

namespace sn
{

class ModuleInfo
{
public:
    bool loadFromFile(const String & pathToProjects, const String & modPath);

    struct Service
    {
        std::string type;
        JsonBox::Value args;
    };

    std::string name;
    String directory;
    String modFilePath;
    std::string scriptNamespace;
    std::vector<String> dependencies;
    String startupScene;

    /// \brief List of global entities to create in the main scene when the module is loaded
    std::vector<Service> services;

	/// \brief List of names of shared libraries containing native bindings
    /// \warning The order is important in this container.
    std::vector<String> bindings;

private:
    void parseServices(JsonBox::Value & o);

};

} // namespace sn


#endif // __HEADER_MODULE_INFO__





