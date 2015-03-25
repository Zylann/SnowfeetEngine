/*
ModuleArgs.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MODULE_ARGS__
#define __HEADER_SN_MODULE_ARGS__

#include <core/config.hpp>
#include <core/util/stringutils.hpp>
#include <core/util/String.hpp>
#include <core/reflect/ObjectTypeDatabase.hpp>
#include <core/squirrel/bind_tools.hpp>
#include <core/app/ModuleInfo.hpp>

/// \brief This struct is passed as single argument of the loading function of a native module.
struct SN_API ModuleLoadArgs
{
	const sn::ModuleInfo * moduleInfo;
    HSQUIRRELVM squirrelVM;
    sn::ObjectTypeDatabase * objectTypeDatabase;

	//ModuleLoadArgs()
	//{
	//	memset(this, 0, sizeof(ModuleLoadArgs));
	//}
};

/// \brief This struct is passed as single argument of the unloading function of a native module.
typedef ModuleLoadArgs ModuleUnloadArgs;
//struct SN_API ModuleUnloadArgs
//{
//	const sn::ModuleInfo * moduleInfo;
//    HSQUIRRELVM squirrelVM;
//    sn::ObjectTypeDatabase * objectTypeDatabase;
//};

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

