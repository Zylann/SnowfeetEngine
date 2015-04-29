#include "mod_TGUI.hpp"

#include "Control.hpp"
#include "GUI.hpp"
#include "Button.hpp"
#include "Renderer.hpp"
#include "Theme.hpp"

namespace tgui
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        otb.registerType<Control>();
        otb.registerType<GUI>();
        otb.registerType<Button>();
        otb.registerType<Renderer>();
        otb.registerType<Theme>();
        otb.registerType<ThemeLoader>();
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

