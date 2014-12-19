/*
ScriptEngine.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SCRIPTENGINE__
#define __HEADER_SCRIPTENGINE__

#include <angelscript.h>
#include <core/angelscript/addons/serializer/serializer.h>
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
    CSerializer * getSerializer();

    bool compileModule(std::string modName, std::string modNamespace, const std::vector<String> & files);

    // Executes a function previously prepared on a context, and returns false if an error occurred.
    bool executeContext(asIScriptContext & context);

private:

    void registerCoreAPI();

    Application & r_app;

    asIScriptEngine * m_engine;
    asIScriptContext * m_context;

    // Serializer used for hot-reload
    CSerializer * m_serializer;

};

/// \brief Checks the return code of an AngelScript method,
/// and throws an exception if an error occurred
void SN_API asCheck(int returnCode);

} // namespace sn

#endif // __HEADER_SCRIPTENGINE__

