#include "mod_TGUI.h"

#include "GUI.h"
#include "Renderer.h"

#include "controls/Control.h"
#include "controls/Button.h"
#include "controls/Text.h"
#include "controls/Panel.h"
#include "controls/Slider.h"
#include "controls/TextArea.h"
#include "controls/Docker.h"
#include "controls/DockSizer.h"

#include "layouts/ListLayout.h"
#include "layouts/GridLayout.h"

#include "theme/Theme.h"

#include "bind/sq_tgui.h"

namespace tgui
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        // Controls
        otb.registerType<Control>();
        otb.registerType<GUI>();
        otb.registerType<Button>();
        otb.registerType<Renderer>();
        otb.registerType<Text>();
        otb.registerType<Panel>();
        otb.registerType<Slider>();
        otb.registerType<TextArea>();
        otb.registerType<Docker>();
		otb.registerType<DockSizer>();

		// Layouts
		otb.registerType<Layout>();
		otb.registerType<ListLayout>();
		otb.registerType<GridLayout>();
		otb.registerType<SplitLayout>();

        // Assets
        otb.registerType<Theme>();
        otb.registerType<ThemeLoader>();
    }
}

int loadSnowfeetModule_ModTGUI(ModuleLoadArgs args)
{
	tgui::registerObjectTypes(*(args.objectTypeDatabase));
    tgui::bindTGUI(args.squirrelVM);
    return 0;
}

int unloadSnowfeetModule_ModTGUI(ModuleUnloadArgs args)
{
    return 0;
}

