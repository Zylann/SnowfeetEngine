#ifndef __HEADER_TGUI_TGUI__
#define __HEADER_TGUI_TGUI__

#include <core/app/ModuleArgs.h>

extern "C"
{
    SN_MOD_EXPORT int loadSnowfeetModule_ModTGUI(ModuleLoadArgs args);
    SN_MOD_EXPORT int unloadSnowfeetModule_ModTGUI(ModuleUnloadArgs args);
}

#endif // __HEADER_TGUI_TGUI__

