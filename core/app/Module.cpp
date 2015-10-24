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
void Module::calculateDependencies(
    const String & pathToProjects,
    const String & modPath,
    std::list<ModuleInfo> & dependencies,
    std::set<String> * openSet)
{
    std::set<String> _openSet;
    if (openSet == nullptr)
    {
        openSet = &_openSet;
    }

    if (openSet->find(modPath) != openSet->end())
    {
        return;
    }

    ModuleInfo info;
    if (!info.loadFromFile(pathToProjects, modPath))
    {
        throw Exception("Unable to load module info \"" + toString(modPath) + '"');
    }

    openSet->insert(modPath);
    dependencies.push_front(info);

    for (auto it = info.dependencies.begin(); it != info.dependencies.end(); ++it)
    {
        String dep = *it;
        calculateDependencies(pathToProjects, dep, dependencies, openSet);
    }

    return;
}

//------------------------------------------------------------------------------
Module::Module(Application & app, const ModuleInfo & info) :
    m_info(info),
    r_app(app)
{
}

//------------------------------------------------------------------------------
Module::~Module()
{
    unloadNativeBindings();
}

//------------------------------------------------------------------------------
bool Module::loadNativeBindings(ScriptManager & scriptEngine)
{
    ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    otb.beginModule(m_info.name);

    bool success = true;

    if (!m_info.bindings.empty())
    {
        // For each registered binding library
        for (u32 i = 0; i < m_info.bindings.size(); ++i)
        {
            String libName = m_info.bindings[i];
            // TODO win32 specific!
            String path = libName + L".dll";

            SN_WLOG(L"Loading shared lib \"" << path << L"\"...");
            std::string loadFuncName = getLoadFuncName(libName);

            // Load the shared library
            SharedLib * lib = new SharedLib();

            if (lib->loadFromFile(path))
            {
                // Get entry point
                NativeModLoadFunc f = (NativeModLoadFunc)(lib->getAddressOf(loadFuncName));
                if (f != nullptr)
                {
                    // Execute entry point
                    int retval = f({
						&m_info,
                        scriptEngine.getVM(),
                        &(ObjectTypeDatabase::get())
                    });

                    if (retval < 0)
                    {
                        SN_ERROR("Native binding loading returned " << retval << ", aborting");
                        delete lib;
                        success = false;
                    }
                    else
                    {
                        m_sharedLibs.push_back(lib);
                    }
                }
                else
                {
                    SN_ERROR("Couldn't find " << loadFuncName << " function, aborting");
                    delete lib;
                    success = false;
                }
            }
            else
            {
                SN_WERROR("An error occurred while attempting to load shared library");
                success = false;
            }
        }
    }

    // Register asset loaders
    AssetDatabase::get().addLoadersFromModule(m_info.name);

    otb.endModule();
    return success;
}

//------------------------------------------------------------------------------
void Module::unloadNativeBindings()
{
    // Note: iterations must be in reverse order because loading order may be important

    // First, call unload functions
    auto libNameIt = m_info.bindings.rbegin();
    for (auto it = m_sharedLibs.rbegin(); it != m_sharedLibs.rend(); ++it, ++libNameIt)
    {
        SharedLib & lib = **it;
        const String & libName = *libNameIt;
        std::string unloadFuncName = getUnloadFuncName(libName);
        NativeModUnloadFunc f = (NativeModUnloadFunc)(lib.getAddressOf(unloadFuncName));

        if (f != nullptr)
        {
            ScriptManager & scriptEngine = r_app.getScriptManager();

            // Execute exit point
            int unloadResult = f({
				&m_info,
                scriptEngine.getVM(),
                &(ObjectTypeDatabase::get())
            });

            if (unloadResult < 0)
            {
                SN_ERROR("Native binding unloading function returned " << unloadResult);
            }
        }
    }

    // Unload asset loaders
    AssetDatabase::get().releaseLoadersFromModule(m_info.name);

    // Unload reflection if any
    ObjectTypeDatabase::get().unregisterModule(m_info.name);

    // Free libraries
    for (s32 i = m_sharedLibs.size()-1; i != -1; --i)
    {
        SharedLib * lib = m_sharedLibs[i];
        const String & libName = m_info.bindings[i];
        SN_WLOG(L"Freeing shared lib \"" << libName << L"\"...");
        if (!lib->unload())
        {
            SN_ERROR("An error occurred while freeing library");
        }
        delete lib;
    }

    //libNameIt = m_info.bindings.rbegin();
    //for (auto it = m_sharedLibs.rbegin(); it != m_sharedLibs.rend(); ++it, ++libNameIt)
    //{
    //    SharedLib * lib = *it;
    //    const String & libName = *libNameIt;
    //    SN_WLOG(L"Freeing shared lib \"" << libName << L"\"...");
    //    if (!lib->unload())
    //    {
    //        SN_ERROR("An error occurred while freeing library");
    //    }
    //    delete lib;
    //}

    m_sharedLibs.clear();
}

//------------------------------------------------------------------------------
void Module::getScriptFiles(
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
bool Module::compileScripts()
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
void Module::createServices(Scene & scene)
{
    for (auto it = m_info.services.begin(); it != m_info.services.end(); ++it)
    {
        const ModuleInfo::Service & serviceInfo = *it;

        std::string typeName = serviceInfo.type;
        if (scene.getChildByType(typeName))
        {
            SN_WARNING("Service " << typeName << " already created");
        }
        else
        {
            SN_LOG("Creating service " << typeName);
            Entity * e = scene.createChild(typeName);
            if (e && it->args.isDictionary())
            {
                SerializationContext context(m_info.name);
                e->unserializeState(serviceInfo.args, context);
				e->propagateOnReady();
            }
        }
    }
}

} // namespace sn

