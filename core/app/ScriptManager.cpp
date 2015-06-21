/*
ScriptManager.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ScriptManager.hpp"
#include "../util/assert.hpp"
#include "../util/stringutils.hpp"
#include "../util/Exception.hpp"
#include "../bind/sq_core.h"
#include "../squirrel/Script.h"
#include <sqstdaux.h>

#include <stdarg.h>
#include <stdio.h>

//#ifdef SQUNICODE 
//#define scvsprintf vwsprintf 
//#else 
//#define scvsprintf vsprintf 
//#endif 

namespace sn
{

    namespace
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
    close();
}

//------------------------------------------------------------------------------
void ScriptManager::initialize()
{
    SN_ASSERT(m_squirrelVM == nullptr, "initialize called twice");

    SN_LOG("Initializing script engine");

    // Create the script engine
    m_squirrelVM = sq_open(1024);

	// Registers default error handlers
	sqstd_seterrorhandlers(m_squirrelVM);

	// Register print and error functions
	sq_setprintfunc(m_squirrelVM, sqPrintfunc, sqErrorfunc);

	// Register API
    registerCoreAPI();
}

//------------------------------------------------------------------------------
void ScriptManager::close()
{
    if (m_squirrelVM)
    {
        sq_close(m_squirrelVM);
        m_squirrelVM = nullptr;
    }
}

//------------------------------------------------------------------------------
bool ScriptManager::compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files)
{
    // Compile scripts

    std::vector<squirrel::Script> scripts;
    std::string sourceCode;

    for (auto it = files.begin(); it != files.end(); ++it)
    {
        const String & filePath = *it;
        if (readFile(toString(filePath), sourceCode))
        {
            squirrel::Script script(m_squirrelVM);
            if (script.compileString(sourceCode))
            {
                scripts.push_back(script);
            }
            else
            {
                SN_ERROR(squirrel::getLastError(script.getVM()));
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    // Run scripts

    for (auto it = scripts.begin(); it != scripts.end(); ++it)
    {
        auto & script = *it;
        script.execute();
    }

    return true;
}

//------------------------------------------------------------------------------
void ScriptManager::registerCoreAPI()
{
    SN_LOG("Registering core API");
    bindCore(m_squirrelVM);
}

} // namespace sn

