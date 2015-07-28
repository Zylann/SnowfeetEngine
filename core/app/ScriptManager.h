/*
ScriptManager.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SCRIPTMANAGER__
#define __HEADER_SCRIPTMANAGER__

//#include <angelscript.h>
//#include <core/angelscript/addons/serializer/serializer.h>
#include <string>
#include <core/util/String.h>
#include <core/squirrel/bind_tools.h>
#include <core/squirrel/Table.h>

namespace sn
{

class Application;

/// \brief Handles functionnality related to scripts
class SN_API ScriptManager
{
public:

    ScriptManager(Application & app);
    ~ScriptManager();

    void initialize();
    void close();

    inline HSQUIRRELVM getVM() const { return m_squirrelVM; }

    bool compileSquirrelModule(const std::string & modName, const std::string & modNamespace, const std::vector<String> & files);

	/// \brief Walk through the root table and find classes in order to map them by name.
	/// (This is because Squirrel classes don't have a name)
	void rebuildClassMapping();

	bool getClassName(HSQOBJECT classObject, std::string & out_className);

private:

    void registerCoreAPI();

    Application & r_app;
    HSQUIRRELVM m_squirrelVM;
	squirrel::Table m_classToName;

};

} // namespace sn

#endif // __HEADER_SCRIPTMANAGER__

