#ifndef __HEADER_SN_MODULE__
#define __HEADER_SN_MODULE__

#include <core/app/ModuleArgs.hpp>
#include <core/app/ModuleInfo.hpp>
#include <core/system/time/Time.hpp>

#include <angelscript.h>

#include <set>
#include <list>
#include <map>

namespace sn
{

namespace CallbackName
{
    extern const char * CREATE;
    extern const char * START;
    extern const char * EVENT;
    extern const char * UPDATE;
    extern const char * DESTROY;
}

class Application;
class ModuleImpl;
class ScriptEngine;

class SN_API Module
{
public:

    Module(Application & app, const ModuleInfo & info);
    ~Module();

    bool loadNativeBindings(ScriptEngine & scriptEngine);
    bool compileScripts();
    bool loadAssets();

    static void calculateDependencies(
        const String & pathToProjects,
        const String & modPath, 
        std::list<ModuleInfo> & dependencies,
        std::set<String> * openSet = nullptr
    );

    bool hasUpdateFunction();

    void callVoidCallback(std::string cbName);
    void onUpdate(Time delta);

    inline const ModuleInfo & getInfo() const { return m_info; }

private:

    void getScriptFiles(std::vector<String> & out_filePaths);

    bool loadNativeBindingsImpl(ScriptEngine & scriptEngine);

    void unloadNativeBindings();
    void unloadNativeBindingsImpl();

    void referenceCallbacks();
    void clearCallbacks();

    std::map<std::string, std::vector<asIScriptFunction*>> m_scriptCallbacks;

    const ModuleInfo m_info;
    Application & r_app;
    ModuleImpl * m_impl;

};

} // namespace sn

#endif // __HEADER_SN_MODULE__

