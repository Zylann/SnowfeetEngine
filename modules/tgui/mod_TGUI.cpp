#include "mod_TGUI.h"

#include "GUI.h"
#include "RenderStep.h"

#include "controls/Control.h"
#include "controls/Button.h"
#include "controls/Text.h"
#include "controls/Panel.h"
#include "controls/Slider.h"
#include "controls/TextArea.h"
#include "controls/Docker.h"
#include "controls/DockSizer.h"
#include "controls/Tab.h"
#include "controls/TabControl.h"

#include "layouts/ListLayout.h"
#include "layouts/GridLayout.h"

#include "theme/Theme.h"

#include "bind/sq_tgui.h"
#include <core/util/macros.h>

namespace tgui
{
    void registerObjectTypes(sn::ObjectTypeDatabase & otb)
    {
        // Controls
        otb.registerType<Control, sn::Entity>              (SN_TYPESTRING(tgui::Control));
        otb.registerType<GUI, Control>                     (SN_TYPESTRING(tgui::GUI));
        otb.registerType<Button, Control>                  (SN_TYPESTRING(tgui::Button));
        otb.registerType<RenderStep, sn::RenderStep>       (SN_TYPESTRING(tgui::RenderStep));
        otb.registerType<Text, Control>                    (SN_TYPESTRING(tgui::Text));
        otb.registerType<Panel, Control>                   (SN_TYPESTRING(tgui::Panel));
        otb.registerType<Slider, Control>                  (SN_TYPESTRING(tgui::Slider));
        otb.registerType<TextArea, Control>                (SN_TYPESTRING(tgui::TextArea));
        otb.registerType<Docker, Control>                  (SN_TYPESTRING(tgui::Docker));
		otb.registerType<DockSizer, Control>               (SN_TYPESTRING(tgui::DockSizer));
        otb.registerType<Tab, Control>                     (SN_TYPESTRING(tgui::Tab));
        otb.registerType<TabControl, Control>              (SN_TYPESTRING(tgui::TabControl));

		// Layouts
		otb.registerType<Layout, sn::Object>               (SN_TYPESTRING(tgui::Layout));
		otb.registerType<ListLayout, Layout>               (SN_TYPESTRING(tgui::ListLayout));
		otb.registerType<GridLayout, Layout>               (SN_TYPESTRING(tgui::GridLayout));
		otb.registerType<SplitLayout, Layout>              (SN_TYPESTRING(tgui::SplitLayout));

        // Assets
        otb.registerType<Theme>                            (SN_TYPESTRING(tgui::Theme));
        otb.registerType<ThemeLoader, sn::AssetLoader>     (SN_TYPESTRING(tgui::ThemeLoader));
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

