/*
ScriptManager.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ScriptManager.h"
#include "../util/assert.h"
#include "../util/stringutils.h"
#include "../util/Exception.h"
#include "../bind/sq_core.h"
#include "../squirrel/Script.h"
#include "../squirrel/Table.h"
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
		const std::string s_classToNameKey = "__classToName";

        //------------------------------------------------------------------------------
        void sqPrintfunc(HSQUIRRELVM v, const SQChar *s, ...)
        {
            // Get the message in a string
            char buffer[2048];
            va_list vl;
            va_start(vl, s);
            vsprintf(buffer, s, vl);
            //printf(s, vl);
            //scvprintf(s, vl);
            va_end(vl);

            // Print
            sn::Log::get().print(SN_LTM_INFO, buffer);
        }

        //------------------------------------------------------------------------------
        void sqErrorfunc(HSQUIRRELVM v, const SQChar *s, ...)
        {
            // Get the message in a string
            char buffer[2048];
            va_list vl;
	        va_start(vl, s);
            vsprintf(buffer, s, vl);
            //printf(s, vl);
	        //scvprintf(s, vl);
	        va_end(vl);

            // Remove the \n Squirrel puts for us in its default error handler
            size_t len = strlen(buffer);
            buffer[len - 1] = '\0';

            // Print
            sn::Log::get().print(SN_LTM_ERROR, buffer);
            //SN_ERROR(buffer);
        }

		//------------------------------------------------------------------------------
		SQRESULT sqGetClassName(HSQUIRRELVM vm)
		{
			HSQOBJECT classObj;
			sq_getstackobj(vm, 2, &classObj);
			if (sq_type(classObj) == OT_INSTANCE)
			{
				sq_getclass(vm, 2);
				sq_getstackobj(vm, -1, &classObj);
			}
			else if (sq_type(classObj) != OT_INSTANCE)
			{
				SN_ERROR("getClassName: expected instance or class object, got type " << sq_type(classObj));
				return 0;
			}

			sq_pushroottable(vm);
			sq_pushstring(vm, s_classToNameKey.c_str(), s_classToNameKey.size());
			if (SQ_FAILED(sq_rawget(vm, -2)))
			{
				SN_ERROR("Failed to retrieve class name from " << s_classToNameKey);
				sq_pop(vm, 1); // pop roottable
				return 0;
			}

			sq_pushobject(vm, classObj);
			if (SQ_FAILED(sq_rawget(vm, -2)))
			{
				sq_pop(vm, 2); // pop classObj and roottable
				return 0;
			}

			const char * classNameCstr = nullptr;
			if (SQ_FAILED(sq_getstring(vm, -1, &classNameCstr)))
			{
				sq_pop(vm, 2); // pop classObj and roottable
				return 0;
			}

			// TODO maybe there is a more optimized way to push that string than copy it

			std::string className = classNameCstr;
			sq_pop(vm, 2); // pop classObj and roottable
			sq_pushstring(vm, className.c_str(), className.size());

			return 1;
		}

    } // anonymous namespace


//==============================================================================
// ScriptManager
//==============================================================================

//------------------------------------------------------------------------------
ScriptManager::ScriptManager(Application & app) :
    r_app(app),
    m_squirrelVM(nullptr)
{
}

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

	rebuildClassMapping();
}

//------------------------------------------------------------------------------
void ScriptManager::close()
{
    if (m_squirrelVM)
    {
		// Release Squirrel objects first to prevent leaks
		m_classToName.releaseObject();

		// Close the Squirrel VM
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
        const std::string & filePath = sn::toString(*it);
        if (readFile(filePath, sourceCode))
        {
            squirrel::Script script(m_squirrelVM);
            if (script.compileString(sourceCode, filePath))
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

    for (size_t i = 0; i < scripts.size(); ++i)
    {
        auto & script = scripts[i];
        script.execute();
    }

    return true;
}

//------------------------------------------------------------------------------
void ScriptManager::rebuildClassMapping()
{
	HSQUIRRELVM vm = m_squirrelVM;

	// Recreate the mapping table
	m_classToName.create(vm);
	squirrel::RootTable root(vm);
	root.setField(s_classToNameKey.c_str(), m_classToName);

	// Iterate through the root table
	sq_pushroottable(vm);
	sq_pushnull(vm); // Push null iterator
	while (SQ_SUCCEEDED(sq_next(vm, -2)))
	{
		// -1 is the value
		// -2 is the key
		const u32 keyIndex = -2;
		const u32 valueIndex = -1;

		HSQOBJECT classObject;
		sq_getstackobj(vm, valueIndex, &classObject);
		if (sq_type(classObject) == OT_CLASS)
		{
			HSQOBJECT key;
			sq_getstackobj(vm, keyIndex, &key);
			if (sq_type(key) == OT_STRING)
			{
				const char * className = nullptr;
				sq_getstring(vm, keyIndex, &className);

				SN_DLOG("Found Squirrel class '" << className << "'");

				// classToName[class] <- name
				m_classToName.setField(classObject, className);
			}
		}
		//else if (sq_type(value) == OT_TABLE)
		//{
		//	// TODO Go recursive?
		//}

		// Pop key and value before next iteration
		sq_pop(vm, 2);
	}
	// Pop null iterator and roottable
	sq_pop(vm, 2);
}

//------------------------------------------------------------------------------
bool ScriptManager::getClassName(HSQOBJECT classObject, std::string & out_className)
{
	return m_classToName.getString(classObject, out_className);
}

//------------------------------------------------------------------------------
void ScriptManager::registerCoreAPI()
{
    SN_LOG("Registering core API");

	squirrel::RootTable root(m_squirrelVM);
	root.setMethod("getClassName", sqGetClassName, 2, "tx|y");

    bindCore(m_squirrelVM);
}

} // namespace sn

