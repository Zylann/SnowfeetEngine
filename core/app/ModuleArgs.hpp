#ifndef __HEADER_SN_MODULE_ARGS__
#define __HEADER_SN_MODULE_ARGS__

// Name of the functions in shared libraries containing a module binding
#define SN_MOD_LOAD_FUNC_NAME "loadSnowfeetModule"
#define SN_MOD_UNLOAD_FUNC_NAME "unloadSnowfeetModule"

#include <core/config.hpp>
#include <angelscript.h>
#include <core/angelscript/addons/serializer/serializer.h>

struct SN_API ModuleLoadArgs
{
    asIScriptEngine * scriptEngine;
    CSerializer * serializer;
};

struct SN_API ModuleUnloadArgs
{
    asIScriptEngine * scriptEngine;
    CSerializer * serializer;
};

namespace sn
{

typedef int(*NativeModLoadFunc)(ModuleLoadArgs);
typedef int(*NativeModUnloadFunc)(ModuleUnloadArgs);

} // namespace sn

#endif // __HEADER_SN_MODULE_ARGS__

