/*
ScriptEngine.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SCRIPTENGINE__
#define __HEADER_SCRIPTENGINE__

#include <angelscript.h>
#include <string>
#include <core/util/String.hpp>

namespace sn
{

class Application;

class SN_API ScriptEngine
{
public:

    ScriptEngine(Application & app);
    ~ScriptEngine();

    void initialize();

    asIScriptEngine * getEngine();
    asIScriptContext * getContext();

    bool compileModule(std::string modName, std::string modNamespace, const std::vector<String> & files);

private:

    void registerCoreAPI();

    Application & r_app;
    asIScriptEngine * m_engine;
    asIScriptContext * m_context;

};

/// \brief Checks the return code of an AngelScript method,
/// and throws an exception if an error occurred
void SN_API asCheck(int returnCode);

} // namespace sn

#endif // __HEADER_SCRIPTENGINE__

