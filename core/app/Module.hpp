#ifndef __HEADER_SN_MODULE__
#define __HEADER_SN_MODULE__

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

class SN_API Module
{
public:

    Module(Application & app, const ModuleInfo & info);
    ~Module();

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

private:

    void referenceCallbacks();
    void clearCallbacks();

    std::map<std::string, std::vector<asIScriptFunction*>> m_scriptCallbacks;

    ModuleInfo m_info;
    Application & r_app;

};

} // namespace sn

#endif // __HEADER_SN_MODULE__

