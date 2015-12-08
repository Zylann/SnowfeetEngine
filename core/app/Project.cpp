/*
Module.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <fstream>

#include "Module.h"
#include "Application.h"

#include <core/asset/AssetDatabase.h>
#include <core/util/stringutils.h>
#include <core/util/Exception.h>
#include <core/system/filesystem.h>
#include <core/system/Time.h>

namespace sn
{

//------------------------------------------------------------------------------
namespace
{
    bool getFilesRecursivelyIgnoringTopDirs(
            String topDirectory, 
            std::vector<FileNode> & out_nodes, 
            std::unordered_set<String> ignoreTopDirs)
    {
        std::vector<FileNode> nodes;
        if (!getFiles(topDirectory, nodes))
            return false;

        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            FileNode & node = *it;

            if (node.isDirectory && ignoreTopDirs.find(node.path) != ignoreTopDirs.end())
                continue;

            node.path = FilePath::join(topDirectory, node.path);

            if (node.isDirectory)
            {
                if (!getFilesRecursively(node.path, out_nodes))
                    return false;
            }
            else
            {
                out_nodes.push_back(node);
            }
        }

        return true;
    }
}

//------------------------------------------------------------------------------
// Static
void Project::calculateProjectDependencies(
    const String & pathToProjects,
    const String & projectDir,
    std::list<ProjectInfo> & dependencies,
    std::set<String> * openSet)
{
    std::set<String> _openSet;
    if (openSet == nullptr)
    {
        openSet = &_openSet;
    }

    if (openSet->find(projectDir) != openSet->end())
    {
        return;
    }

    ProjectInfo info;
    if (!info.loadFromFile(pathToProjects, FilePath::join(projectDir, toWideString(ProjectInfo::FILE_NAME))))
    {
        throw Exception("Unable to load project info for \"" + toString(projectDir) + '"');
    }

    openSet->insert(projectDir);
    dependencies.push_front(info);

    for (auto it = info.dependencies.begin(); it != info.dependencies.end(); ++it)
    {
        String dep = *it;
        calculateProjectDependencies(pathToProjects, dep, dependencies, openSet);
    }

    return;
}

//------------------------------------------------------------------------------
Project::Project(Application & app, const ProjectInfo & info) :
    m_info(info),
    r_app(app)
{
}

//------------------------------------------------------------------------------
Project::~Project()
{
}

//------------------------------------------------------------------------------
bool Project::loadModules()
{
    bool success = true;

    // For each registered binding library
    for (u32 i = 0; i < m_info.modules.size(); ++i)
    {
        std::string libName = m_info.modules[i];
        // TODO win32 specific!
        std::string path = libName + ".dll";

        if (!r_app.loadModule(path))
            success = false;
    }

    return success;
}

//------------------------------------------------------------------------------
void Project::getScriptFiles(
        std::vector<String> & out_filePaths, 
        const std::set<String> & extensions,
        const std::unordered_set<String> & ignoreDirectories)
{
    String pathBase = r_app.getPathToProjects() + L"/" + m_info.directory;

    std::vector<FileNode> files;
    getFilesRecursivelyIgnoringTopDirs(pathBase, files, ignoreDirectories);

    // Retrieve paths
    for (u32 i = 0; i < files.size(); ++i)
    {
        FileNode & node = files[i];
        SN_ASSERT(!node.isDirectory, "Unexpected directory");
        String ext = getFileExtension(node.path);
        if (extensions.find(ext) != extensions.end())
        {
            out_filePaths.push_back(node.path);
        }
    }
}

//------------------------------------------------------------------------------
bool Project::compileScripts()
{
    // Get script files
    std::vector<String> scriptFiles;
    std::set<String> exts;
    exts.insert(L".nut");
    std::unordered_set<String> ignoreDirs;
    ignoreDirs.insert(L"cpp");
    getScriptFiles(scriptFiles, exts, ignoreDirs);

    if (!scriptFiles.empty())
    {
        // Compile
        bool compiled = r_app.getScriptManager().compileSquirrelModule(
            m_info.name,
            m_info.scriptNamespace,
            scriptFiles
        );

        if (!compiled)
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
//void Project::createServices(Scene & scene)
//{
//    SerializationContext context(m_info.name);
//
//    for (auto it = m_info.services.begin(); it != m_info.services.end(); ++it)
//    {
//        const ProjectInfo::Service & serviceInfo = *it;
//
//        std::string typeName = serviceInfo.type;
//        if (scene.getChildByType(typeName))
//        {
//            SN_WARNING("Service " << typeName << " already created");
//        }
//        else
//        {
//            SN_LOG("Creating service " << typeName);
//            Entity * e = scene.createChild(typeName);
//            if (e && it->args.isDictionary())
//            {
//                e->unserializeState(serviceInfo.args, context);
//				e->propagateOnReady();
//            }
//        }
//    }
//}

} // namespace sn

