#ifndef __HEADER_SN_SYSTEMDIALOG__
#define __HEADER_SN_SYSTEMDIALOG__

#include <core/app/ModuleArgs.h>
#include <core/config.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModSystemDialog(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModSystemDialog(ModuleUnloadArgs args);
}

#endif // __HEADER_SN_SYSTEMDIALOG__

