/*
ScriptEngine.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SCRIPTENGINE__
#define __HEADER_SCRIPTENGINE__

//#include <angelscript.h>
//#include <core/angelscript/addons/serializer/serializer.h>
#include <string>
#include <core/util/String.hpp>
#include <core/squirrel/bind_tools.hpp>

namespace sn
{

class Application;

/// \brief Handles functionnality related to scripts
class SN_API ScriptEngine
{
public:

    ScriptEngine(Application & app);
    ~ScriptEngine();

    void initialize();

    inline HSQUIRRELVM getVM() const { return m_squirrelVM; }

    bool compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files);

private:

    void registerCoreAPI();

    Application & r_app;
    HSQUIRRELVM m_squirrelVM;

};

} // namespace sn

#endif // __HEADER_SCRIPTENGINE__

