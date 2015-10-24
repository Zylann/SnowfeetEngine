/*
ModuleInfo.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_MODULE_INFO__
#define __HEADER_MODULE_INFO__

#include <core/util/String.h>
#include <core/util/Variant.h>
#include <vector>

namespace sn
{

class ModuleInfo
{
public:
    bool loadFromFile(const String & pathToProjects, const String & modPath);

    struct Service
    {
        std::string type;
        Variant args;
    };

    std::string name;
    String directory;
    String modFilePath;

    std::string scriptNamespace;

    /// \brief Dependency projects required by this project
    std::vector<String> dependencies;

    /// \brief Native libraries required by the project
    std::vector<std::string> modules;

    /// \brief Name of the startup scene asset.
    std::string startupScene;

    /// \brief List of global entities to create in the main scene when the module is loaded
    std::vector<Service> services;

	/// \brief List of names of shared libraries containing native bindings
    /// \warning The order is important in this container.
    std::vector<String> bindings;

    /// \brief Serialized form of the update manager
    Variant updateLayers;

private:
    bool setDataFromVariant(const Variant & v);
    void parseServices(const Variant & o);

};

} // namespace sn


#endif // __HEADER_MODULE_INFO__





