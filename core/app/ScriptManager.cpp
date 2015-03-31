/*
ScriptManager.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ScriptManager.hpp"
#include "../util/assert.hpp"
#include "../util/stringutils.hpp"
#include "../util/Exception.hpp"
#include "../scene/sq_binding/sq_scene.hpp"

#include <stdarg.h>
#include <stdio.h>

//#ifdef SQUNICODE 
//#define scvsprintf vwsprintf 
//#else 
//#define scvsprintf vsprintf 
//#endif 

namespace sn
{

//------------------------------------------------------------------------------
void sqPrintfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
    char buffer[2048];
    va_list vl;
    va_start(vl, s);
    vsprintf(buffer, s, vl);
    //printf(s, vl);
    //scvprintf(s, vl);
    va_end(vl);
    SN_LOG(buffer);
}

//------------------------------------------------------------------------------
void sqErrorfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
    char buffer[2048];
    va_list vl;
	va_start(vl, s);
    vsprintf(buffer, s, vl);
    //printf(s, vl);
	//scvprintf(s, vl);
	va_end(vl);
    SN_ERROR(buffer);
}

//==============================================================================
// ScriptManager
//==============================================================================

//------------------------------------------------------------------------------
ScriptManager::ScriptManager(Application & app) :
    r_app(app),
    m_squirrelVM(nullptr)
{}

//------------------------------------------------------------------------------
ScriptManager::~ScriptManager()
{
    if (m_squirrelVM)
    {
        sq_close(m_squirrelVM);
    }
}

//------------------------------------------------------------------------------
void ScriptManager::initialize()
{
    SN_ASSERT(m_squirrelVM == nullptr, "initialize called twice");

    SN_LOG("Initializing script engine");

    // Create the script engine
    m_squirrelVM = sq_open(1024);
    Sqrat::DefaultVM::Set(m_squirrelVM);

	// Registers default error handlers
	sqstd_seterrorhandlers(m_squirrelVM);

	// Register print and error functions
	sq_setprintfunc(m_squirrelVM, sqPrintfunc, sqErrorfunc);

	// Register API
    registerCoreAPI();
}

//------------------------------------------------------------------------------
bool ScriptManager::compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files)
{
    // Compile scripts
    std::vector<Sqrat::Script> scripts;
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        const String & filePath = *it;
        try
        {
            Sqrat::Script script(m_squirrelVM);
            script.CompileFile(toString(filePath));
            scripts.push_back(script);
        }
        catch (Sqrat::Exception ex)
        {
            SN_ERROR(ex.Message());
            return false;
        }
    }

    // Run scripts
    for (auto it = scripts.begin(); it != scripts.end(); ++it)
    {
        auto & script = *it;
        script.Run();
    }

    return true;
}

//------------------------------------------------------------------------------
void ScriptManager::registerCoreAPI()
{
    SN_LOG("Registering core API");

    // Register math functions
    // TODO

    // Register print functions
    // TODO

    // Time
    // TODO
    //register_Time(m_engine);
    //register_Clock(m_engine);

    // Console stuff
    // TODO
	//register_console(*m_engine);

    // Register Entity system
    registerEntity(m_squirrelVM);
    registerScene(m_squirrelVM);
}

} // namespace sn

