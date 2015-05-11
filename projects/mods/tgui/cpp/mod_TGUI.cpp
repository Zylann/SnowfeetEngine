#include "mod_TGUI.h"

#include "Control.h"
#include "GUI.h"
#include "Button.h"
#include "Text.h"
#include "Panel.hpp"
#include "Renderer.h"
#include "Theme.h"

namespace tgui
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        // Entities
        otb.registerType<Control>();
        otb.registerType<GUI>();
        otb.registerType<Button>();
        otb.registerType<Renderer>();
        otb.registerType<Text>();
        otb.registerType<Panel>();

        // Assets
        otb.registerType<Theme>();
        otb.registerType<ThemeLoader>();
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

