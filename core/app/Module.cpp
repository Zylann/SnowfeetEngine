#include "Module.hpp"
#include "Application.hpp"
#include <JsonBox.h>
#include <fstream>
#include <core/util/stringutils.hpp>
#include <core/util/Exception.hpp>
#include <core/system/file/filesystem.hpp>

namespace sn
{

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
}

//------------------------------------------------------------------------------
bool Module::compileScripts()
{
    String fullDirectoryPath = r_app.getPathToProjects() + L"/" + m_info.directory;
    std::vector<FileNode> files;
    getFiles(fullDirectoryPath, files);

    std::vector<String> scriptFiles;
    for (u32 i = 0; i < files.size(); ++i)
    {
        if (!files[i].isDirectory)
        {
            String path = files[i].path;
            String ext = getFileExtension(path);
            if (ext == L".ac" || ext == L".as" || ext == L".meow")
            {
                scriptFiles.push_back(r_app.getPathToProjects() + L"/" + m_info.directory + L"/" + path);
            }
        }
    }

    return r_app.getScriptEngine().compileModule(
        m_info.name,
        m_info.scriptNamespace,
        scriptFiles
    );
}

//------------------------------------------------------------------------------
bool Module::loadAssets()
{
    return true;
}


} // namespace sn

