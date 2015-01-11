#include "../Module.hpp"
#include "../ScriptEngine.hpp"
#include "../../util/assert.hpp"
#include "../Application.hpp"
#include <Windows.h>

namespace sn
{

//------------------------------------------------------------------------------
// TODO Cross-platform SharedLib class?
class SharedLib
{
public:
    SharedLib(const String & name_, HINSTANCE instance_) :
        name(name_),
        instance(instance_)
    {}

    String name;
    HINSTANCE instance;
};

//------------------------------------------------------------------------------
class ModuleImpl
{
public:
    //! IMPORTANT: In .mod binding list order !//
    std::vector<SharedLib> sharedLibs;
};

//------------------------------------------------------------------------------
bool Module::loadNativeBindingsImpl(ScriptEngine & scriptEngine)
{
    SN_ASSERT(m_impl == nullptr, "native bindings loaded twice");

    if (!m_info.bindings.empty())
    {
        if (m_impl == nullptr)
        {
            m_impl = new ModuleImpl();
        }

        String basePath = r_app.getPathToProjects() + L"/" + m_info.directory;

        // For each registered binding library
        for (u32 i = 0; i < m_info.bindings.size(); ++i)
        {
            String libName = m_info.bindings[i];
            String path = basePath + L"/" + libName + L".dll";

            SN_WLOG(L"Loading shared lib \"" << path << L"\"...");
            std::string loadFuncName = getLoadFuncName(libName);

            // Load the shared library
            HINSTANCE hLib = LoadLibraryW(path.c_str());

            if (hLib)
            {
                // Get entry point
                NativeModLoadFunc f = (NativeModLoadFunc)GetProcAddress(hLib, loadFuncName.c_str());
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
                        FreeLibrary(hLib);
                        return false;
                    }

                    m_impl->sharedLibs.push_back(SharedLib(libName, hLib));
                }
                else
                {
                    SN_ERROR("Couldn't find " << loadFuncName << " function, aborting");
                    FreeLibrary(hLib);
                    return false;
                }
            }
            else
            {
                SN_WERROR("An error occurred while attempting to load shared library");
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
void Module::unloadNativeBindingsImpl()
{
    if (m_impl)
    {
        // Note: iterations must be in reverse order because loading order may be important

        // First, call unload functions
        for (auto it = m_impl->sharedLibs.rbegin(); it != m_impl->sharedLibs.rend(); ++it)
        {
            SharedLib & lib = *it;
            std::string unloadFuncName = getUnloadFuncName(lib.name);
            NativeModUnloadFunc f = (NativeModUnloadFunc)GetProcAddress(lib.instance, unloadFuncName.c_str());

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
        for (auto it = m_impl->sharedLibs.rbegin(); it != m_impl->sharedLibs.rend(); ++it)
        {
            SharedLib & lib = *it;
            SN_WLOG(L"Freeing shared lib \"" << lib.name << L"\"...");
            BOOL freeResult = FreeLibrary(lib.instance);
            if (freeResult == 0)
            {
                SN_ERROR("An error occurred while freeing library");
            }
        }

        // Delete implementation
        delete m_impl;
        m_impl = nullptr;
    }
}

} // namespace sn



