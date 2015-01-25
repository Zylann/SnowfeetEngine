#ifndef __HEADER_SN_MODULE_ARGS__
#define __HEADER_SN_MODULE_ARGS__

//#include <angelscript.h>

#include <core/config.hpp>
#include <core/util/stringutils.hpp>
#include <core/util/String.hpp>
#include <core/reflect/ObjectTypeDatabase.hpp>
#include <core/squirrel/bind_tools.hpp>
//#include <core/angelscript/addons/serializer/serializer.h>

#include <functional>

/// \brief This struct is passed as single argument of the loading function of a native module.
struct SN_API ModuleLoadArgs
{
    //asIScriptEngine * scriptEngine;
    //CSerializer * serializer;
    HSQUIRRELVM squirrelVM;
    sn::ObjectTypeDatabase * objectTypeDatabase;
};

/// \brief This struct is passed as single argument of the unloading function of a native module.
struct SN_API ModuleUnloadArgs
{
    //asIScriptEngine * scriptEngine;
    //CSerializer * serializer;
    HSQUIRRELVM squirrelVM;
    sn::ObjectTypeDatabase * objectTypeDatabase;
};

namespace sn
{

//------------------------------------------------------------------------------
typedef int(*NativeModLoadFunc)(ModuleLoadArgs);
typedef int(*NativeModUnloadFunc)(ModuleUnloadArgs);

//------------------------------------------------------------------------------
/// \brief Gets the name of the loading function from the name of a Snowfeet module native lib
inline std::string getLoadFuncName(const String & libName)
{
	return "loadSnowfeetModule_" + toString(libName);
}

//------------------------------------------------------------------------------
/// \brief Gets the name of the unloading function from the name of a Snowfeet module native lib
inline std::string getUnloadFuncName(const String & libName)
{
	return "unloadSnowfeetModule_" + toString(libName);
}

} // namespace sn

#endif // __HEADER_SN_MODULE_ARGS__

