#ifndef __HEADER_SN_MODULE__
#define __HEADER_SN_MODULE__

#include <core/app/ModuleInfo.hpp>
#include <set>
#include <list>

namespace sn
{

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

private:

    ModuleInfo m_info;
    Application & r_app;

};

} // namespace sn

#endif // __HEADER_SN_MODULE__

