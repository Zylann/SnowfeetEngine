/*
Module.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_PROJECT__
#define __HEADER_SN_PROJECT__

#include <core/app/ModuleArgs.h>
#include <core/app/ProjectInfo.h>
#include <core/system/Time.h>
#include <core/system/SharedLib.h>

#include <set>
#include <unordered_set>
#include <list>
#include <map>

namespace sn
{

class Application;
class ScriptManager;
class Scene;

/// \brief A Project is a collection of assets making up an application or framework.
/// It can also require modules, which are native libraries next to the engine's executable.
class SN_API Project
{
public:

    Project(Application & app, const ProjectInfo & info);
    ~Project();

    bool loadModules();
    bool compileScripts();

    /// \brief Calculates the project dependency list of the given project.
    /// The list is ordered by module loading.
    static void calculateProjectDependencies(
        const String & pathToProjects,
        const String & projectPath, 
        std::list<ProjectInfo> & dependencies,
        std::set<String> * openSet = nullptr
    );

    inline const ProjectInfo & getInfo() const { return m_info; }

private:

    void getScriptFiles(
        std::vector<String> & out_filePaths, 
        const std::set<String> & extensions, 
        const std::unordered_set<String> & ignoreDirectories
    );

private:
    const ProjectInfo m_info;
    Application & r_app;

};

} // namespace sn

#endif // __HEADER_SN_PROJECT__

