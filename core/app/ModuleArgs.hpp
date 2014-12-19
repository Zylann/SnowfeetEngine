#ifndef __HEADER_SN_MODULE_ARGS__
#define __HEADER_SN_MODULE_ARGS__

#include <core/config.hpp>
#include <core/util/stringutils.hpp>
#include <core/util/String.hpp>
#include <angelscript.h>
#include <core/angelscript/addons/serializer/serializer.h>

/// \brief This struct is passed as single argument of the loading function of a native module.
struct SN_API ModuleLoadArgs
{
    asIScriptEngine * scriptEngine;
    CSerializer * serializer;
};

/// \brief This struct is passed as single argument of the unloading function of a native module.
struct SN_API ModuleUnloadArgs
{
    asIScriptEngine * scriptEngine;
    CSerializer * serializer;
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
	return "loadSnowfeetModule_" + toString(libName);
}

} // namespace sn

#endif // __HEADER_SN_MODULE_ARGS__

