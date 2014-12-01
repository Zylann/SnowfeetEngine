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
            String name = m_info.bindings[i];
            String path = basePath + L"/" + name + L".dll";

            SN_WLOG(L"Loading shared lib \"" << path << L"\"...");

            // Load the shared library
            HINSTANCE hLib = LoadLibraryW(path.c_str());

            if (hLib)
            {
                // Get entry point
                NativeModLoadFunc f = (NativeModLoadFunc)GetProcAddress(hLib, SN_MOD_LOAD_FUNC_NAME);
                if (f != nullptr)
                {
                    // Execute entry point
                    int retval = f({ scriptEngine.getEngine(), scriptEngine.getSerializer() });

                    if (retval < 0)
                    {
                        SN_ERROR("Native binding loading returned " << retval << ", aborting");
                        FreeLibrary(hLib);
                        return false;
                    }

                    m_impl->sharedLibs.push_back(SharedLib(name, hLib));
                }
                else
                {
                    SN_WERROR("Couldn't find " << SN_MOD_LOAD_FUNC_NAME << " function, aborting");
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
            NativeModUnloadFunc f = (NativeModUnloadFunc)GetProcAddress(lib.instance, SN_MOD_UNLOAD_FUNC_NAME);
            if (f != nullptr)
            {
                ScriptEngine & scriptEngine = r_app.getScriptEngine();

                // Execute exit point
                int unloadResult = f({ scriptEngine.getEngine(), scriptEngine.getSerializer() });

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



