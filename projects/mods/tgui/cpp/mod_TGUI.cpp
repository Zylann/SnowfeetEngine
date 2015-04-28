#include "mod_TGUI.hpp"

#include "Control.hpp"
#include "GUI.hpp"

namespace tgui
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<Control>();
        otb.registerType<GUI>();
        //...
    }
}

int loadSnowfeetModule_ModTGUI(ModuleLoadArgs args)
{
	tgui::registerObjectTypes(*(args.objectTypeDatabase));
    return 0;
}

int unloadSnowfeetModule_ModTGUI(ModuleUnloadArgs args)
{
    return 0;
}

