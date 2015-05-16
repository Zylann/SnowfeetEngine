#include "mod_TGUI.h"

#include "GUI.h"
#include "Renderer.h"

#include "controls/Control.h"
#include "controls/Button.h"
#include "controls/Text.h"
#include "controls/Panel.h"
#include "controls/Slider.h"

#include "theme/Theme.h"

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
        otb.registerType<Slider>();

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

