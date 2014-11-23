#include <JsonBox.h>
#include <fstream>

#include "Module.hpp"
#include "Application.hpp"

#include "../util/stringutils.hpp"
#include "../util/Exception.hpp"
#include "../system/file/filesystem.hpp"
#include "../system/time/Time.hpp"

namespace sn
{

//------------------------------------------------------------------------------
namespace CallbackName
{
    const char * CREATE = "onCreate";
    const char * START = "onStart";
    const char * EVENT = "onEvent";
    const char * UPDATE = "onUpdate";
    const char * DESTROY = "onDestroy";
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
    m_scriptCallbacks.insert(std::make_pair(CallbackName::CREATE, std::vector<asIScriptFunction*>()));
    m_scriptCallbacks.insert(std::make_pair(CallbackName::EVENT, std::vector<asIScriptFunction*>()));
    m_scriptCallbacks.insert(std::make_pair(CallbackName::START, std::vector<asIScriptFunction*>()));
    m_scriptCallbacks.insert(std::make_pair(CallbackName::UPDATE, std::vector<asIScriptFunction*>()));
    m_scriptCallbacks.insert(std::make_pair(CallbackName::DESTROY, std::vector<asIScriptFunction*>()));
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

    bool compiled = r_app.getScriptEngine().compileModule(
        m_info.name,
        m_info.scriptNamespace,
        scriptFiles
    );

    if (compiled)
    {
        referenceCallbacks();
    }
    else
    {
        throw Exception("Script compilation error");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void Module::clearCallbacks()
{
    for (auto it = m_scriptCallbacks.begin(); it != m_scriptCallbacks.end(); ++it)
    {
        it->second.clear();
    }
}

//------------------------------------------------------------------------------
void Module::referenceCallbacks()
{
    SN_DLOG("Referencing callbacks...");

    clearCallbacks();

    asIScriptEngine * as = r_app.getScriptEngine().getEngine();
    asIScriptModule * asModule = as->GetModule(m_info.name.c_str());

    // TODO Handle multiple callback signatures

    // For each global function defined in the module
    u32 fnCount = asModule->GetFunctionCount();
    for (u32 i = 0; i < fnCount; ++i)
    {
        // Get function
        asIScriptFunction * f = asModule->GetFunctionByIndex(i);

        for (auto it = m_scriptCallbacks.begin(); it != m_scriptCallbacks.end(); ++it)
        {
            std::string callbackName = it->first;

            // If the function name matches a callback, reference it
            if (f->GetName() == callbackName)
            {
                it->second.push_back(f);
                SN_DLOG("Found callback \"" << f->GetNamespace() << "::" << callbackName << "\"");
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
bool Module::hasUpdateFunction()
{
    return m_scriptCallbacks[CallbackName::UPDATE].size() > 0;
}

//------------------------------------------------------------------------------
bool Module::loadAssets()
{
    return true;
}

//------------------------------------------------------------------------------
void Module::callVoidCallback(std::string cbName)
{
    auto it = m_scriptCallbacks.find(cbName);
    if (it != m_scriptCallbacks.end())
    {
        asIScriptContext * context = r_app.getScriptEngine().getContext();

        // For each function registered for this callback
        for (u32 i = 0; i < it->second.size(); ++i)
        {
            // Prepare
            asIScriptFunction * f = it->second[i];
            context->Prepare(f);

            // Execute
            r_app.getScriptEngine().executeContext(*context);
        }
    }
}

//------------------------------------------------------------------------------
void Module::onUpdate(Time delta)
{
    auto it = m_scriptCallbacks.find(CallbackName::UPDATE);
    if (it != m_scriptCallbacks.end())
    {
        asIScriptContext * context = r_app.getScriptEngine().getContext();

        // For each update function registered for this callback
        for (u32 i = 0; i < it->second.size(); ++i)
        {
            // Prepare function
            asIScriptFunction * f = it->second[i];
            context->Prepare(f);

            // Set arguments
            context->SetArgObject(0, &delta);

            // Execute
            r_app.getScriptEngine().executeContext(*context);
        }
    }
}

} // namespace sn

