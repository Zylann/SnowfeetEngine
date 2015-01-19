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
    r_app(app),
    m_impl(nullptr)
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
    unloadNativeBindings();
}

//------------------------------------------------------------------------------
bool Module::loadNativeBindings(ScriptEngine & scriptEngine)
{
    ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    otb.beginModule(m_info.name);

    bool success = true;

    if (!m_info.bindings.empty())
    {
        String basePath = r_app.getPathToProjects() + L"/" + m_info.directory;

        // For each registered binding library
        for (u32 i = 0; i < m_info.bindings.size(); ++i)
        {
            String libName = m_info.bindings[i];
            String path = basePath + L"/" + libName + L".dll";

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
                        scriptEngine.getEngine(),
                        scriptEngine.getSerializer(),
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
            ScriptEngine & scriptEngine = r_app.getScriptEngine();

            // Execute exit point
            int unloadResult = f({
                scriptEngine.getEngine(),
                scriptEngine.getSerializer(),
                &(ObjectTypeDatabase::get())
            });

            if (unloadResult < 0)
            {
                SN_ERROR("Native binding unloading function returned " << unloadResult);
            }
        }
    }

    // Unload reflection if any
    ObjectTypeDatabase::get().unregisterModule(m_info.name);

    // Free libraries
    libNameIt = m_info.bindings.rbegin();
    for (auto it = m_sharedLibs.rbegin(); it != m_sharedLibs.rend(); ++it, ++libNameIt)
    {
        SharedLib & lib = **it;
        const String & libName = *libNameIt;
        SN_WLOG(L"Freeing shared lib \"" << libName << L"\"...");
        if (!lib.unload())
        {
            SN_ERROR("An error occurred while freeing library");
        }
        delete *it;
    }

    m_sharedLibs.clear();
}

//------------------------------------------------------------------------------
void Module::getScriptFiles(std::vector<String> & out_filePaths, const std::set<String> & extensions)
{
    String fullDirectoryPath = r_app.getPathToProjects() + L"/" + m_info.directory;
    std::vector<FileNode> files;
    getFiles(fullDirectoryPath, files);

    for (u32 i = 0; i < files.size(); ++i)
    {
        if (!files[i].isDirectory)
        {
            String path = files[i].path;
            String ext = getFileExtension(path);
            if (extensions.find(ext) != extensions.end())
            {
                out_filePaths.push_back(r_app.getPathToProjects() + L"/" + m_info.directory + L"/" + path);
            }
        }
    }
}

//------------------------------------------------------------------------------
bool Module::compileScripts()
{
    // Get script files
    std::vector<String> scriptFiles;
    std::set<String> exts;
    exts.insert(L".ac");
    exts.insert(L".as");
    getScriptFiles(scriptFiles, exts);

    // Compile
    bool compiled = r_app.getScriptEngine().compileAngelscriptModule(
        m_info.name,
        m_info.scriptNamespace,
        scriptFiles
    );

    // Reference callbacks
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

