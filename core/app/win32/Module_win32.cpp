#include "../Module.hpp"
#include "../ScriptEngine.hpp"
#include "../../util/assert.hpp"
#include "../Application.hpp"
#include <Windows.h>

#define SN_MOD_ENTRY_FUNC_NAME "registerSnowfeetModule"

namespace sn
{

//------------------------------------------------------------------------------
typedef int(*NativeModFunc)(asIScriptEngine*);

//------------------------------------------------------------------------------
class ModuleImpl
{
public:
    std::map<String, HINSTANCE> sharedLibs;
};

//------------------------------------------------------------------------------
bool Module::loadNativeBindings(ScriptEngine & scriptEngine)
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
        for (auto it = m_info.bindings.begin(); it != m_info.bindings.end(); ++it)
        {
            String name = *it;
            String path = basePath + L"/" + name + L".dll";

            SN_WLOG(L"Loading shared lib \"" << path << L"\"...");

            // Load the shared library
            HINSTANCE hLib = LoadLibraryW(path.c_str());

            if (hLib)
            {
                // Get entry point
                NativeModFunc f = (NativeModFunc)GetProcAddress(hLib, SN_MOD_ENTRY_FUNC_NAME);
                if (f != nullptr)
                {
                    // Execute entry point
                    int retval = f(scriptEngine.getEngine());

                    if (retval < 0)
                    {
                        SN_ERROR("Native binding registering returned " << retval << ", aborting");
                        FreeLibrary(hLib);
                        return false;
                    }

                    m_impl->sharedLibs.insert(std::make_pair(name, hLib));
                }
                else
                {
                    SN_WERROR("Couldn't find " << SN_MOD_ENTRY_FUNC_NAME << " function, aborting");
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
void Module::unloadNativeBindings()
{
    if (m_impl)
    {
        for (auto it = m_impl->sharedLibs.begin(); it != m_impl->sharedLibs.end(); ++it)
        {
            SN_WLOG("Unloading shared lib \"" << it->first << "\"...");
            HINSTANCE hLib = it->second;
            BOOL retval = FreeLibrary(hLib);
            if (retval == 0)
            {
                SN_ERROR("An error occurred while freeing library");
            }
        }

        delete m_impl;
        m_impl = nullptr;
    }
}

} // namespace sn



