/*
ScriptManager.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ScriptManager.h"
#include "ScriptPreprocessor.h"
#include "../system/FilePath.h"
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
namespace
{
    typedef std::vector<ScriptPreprocessor::RequireItem> ScriptRequireList;
    
    struct ScriptUnit
    {
    	squirrel::Script script;
    	std::string filePath;
    	ScriptRequireList requireItems;
    	std::vector<u32> requireIndexes;
    
    	ScriptUnit(HSQUIRRELVM vm): script(vm) {}
    };
    
	void priv_calculateScriptDependencies(
		u32 i,
		const std::vector<ScriptUnit> units,
		std::vector<u32> & out_order,
		std::vector<bool> & openSet)
	{
		const ScriptUnit & unit = units[i];

		// For each required scripts
		const std::vector<u32> & requireIndexes = unit.requireIndexes;
		for (u32 i = 0; i < requireIndexes.size(); ++i)
		{
			u32 j = requireIndexes[i];
			if (!openSet[j])
			{
				// Include the dependency
				priv_calculateScriptDependencies(j, units, out_order, openSet);
			}
		}

		openSet[i] = true;

		// Include the script
		out_order.push_back(i);
	}

	void calculateScriptDependencies(
		const std::vector<ScriptUnit> & units, 
		std::vector<u32> & out_order)
	{
		std::vector<bool> openSet(units.size(), false);

		// For each script
		for (u32 i = 0; i < units.size(); ++i)
		{
			// If not included yet
			if (!openSet[i])
			{
				// Include it
				priv_calculateScriptDependencies(0, units, out_order, openSet);
			}
		}
	}
}

//------------------------------------------------------------------------------
bool ScriptManager::compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files)
{
    ScriptPreprocessor preprocessor;
    
    // Compile scripts

    std::vector<ScriptUnit> units;
	std::unordered_map<std::string, u32> unitsByFile;
    std::string sourceCode;

    for (auto it = files.begin(); it != files.end(); ++it)
    {
        const std::string & filePath = sn::toString(*it);
        if (readFile(filePath, sourceCode))
        {
            preprocessor.run(sourceCode);
            
            ScriptUnit unit(m_squirrelVM);
            if (unit.script.compileString(sourceCode, filePath))
            {
				unitsByFile[unit.filePath] = units.size();
				unit.filePath = filePath;
				unit.requireItems = preprocessor.getRequireItems();
				units.push_back(unit);
            }
            else
            {
                SN_ERROR(squirrel::getLastError(unit.script.getVM()));
                return false;
            }
        }
        else
        {
            return false;
        }
    }

	// Identify require items

	for (u32 i = 0; i < units.size(); ++i)
	{
		ScriptUnit & unit = units[i];
		for (u32 j = 0; j < unit.requireItems.size(); ++j)
		{
			ScriptPreprocessor::RequireItem & requireItem = unit.requireItems[j];
			std::string unitDirectory = sn::getFileFolder(unit.filePath);
			std::string path = FilePath::join(unitDirectory, requireItem.fileName);

			auto it = unitsByFile.find(path);
			if (it != unitsByFile.end())
			{
				unit.requireIndexes.push_back(it->second);
			}
			else
			{
				SN_ERROR(unit.filePath << ": " << requireItem.line << ": cannot find file " << path);
				return false;
			}
		}
	}

	// Calculate dependencies

	std::vector<u32> runOrder;
	calculateScriptDependencies(units, runOrder);

    // Run scripts

    for (size_t i = 0; i < units.size(); ++i)
    {
#ifdef SN_BUILD_DEBUG
		SN_LOG("Running " << units[i].filePath);
#endif
        auto & script = units[i].script;
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

