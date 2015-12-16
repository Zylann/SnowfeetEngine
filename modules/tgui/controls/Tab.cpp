#include <core/util/Profiler.h>

#include "Tab.h"
#include "TabControl.h"
#include "../GUI.h"
#include "../layouts/ListLayout.h"

using namespace sn;

namespace tgui
{

SN_OBJECT_IMPL(Tab)

void Tab::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
	Control::serializeState(o, ctx);
}

void Tab::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
	Control::unserializeState(o, ctx);
    // Default layout
    if (!hasLayout())
    {
        ListLayout * layout = new ListLayout();
        layout->setOrientation(TGUI_HORIZONTAL);
        setLayout(layout);
    }
}

void Tab::onMouseMove(Event & e)
{
}

void Tab::onMousePress(Event & e)
{
	TabControl * container = Object::cast<TabControl>(getParent());
	if (container)
	{
		u32 i = container->getTabIndex(this);
		if (i != TabControl::INVALID_PAGE)
		{
			container->setCurrentPageIndex(i);
		}
	}
	e.consume();
}

void Tab::onMouseRelease(Event & e)
{
}

void Tab::onDrawSelf(DrawBatch & batch)
{
    GUI * gui = getGUI();
    if (gui == nullptr)
        return;
    const Theme & theme = gui->getTheme();
    const ControlTheme & ct = theme.controlTheme;

    SN_BEGIN_PROFILE_SAMPLE_NAMED("TGUI draw Button");

    IntRect bounds = getClientBounds();

    u32 state = ControlTheme::STATE_NORMAL;
    if (isPressed())
        state = ControlTheme::STATE_PRESSED;
    else if (isHovered())
        state = ControlTheme::STATE_HOVERED;

    const IntRect & uvRect = ct.statesUV[state];
    Vector2u ts = theme.getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);

    SN_END_PROFILE_SAMPLE();
}

} // namespace tgui

