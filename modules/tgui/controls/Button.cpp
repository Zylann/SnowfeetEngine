#include <core/util/Profiler.h>

#include "Button.h"
#include "../GUI.h"
#include "../DrawBatch.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
void Button::onMousePress(Event & e)
{
	if (!getScript().isNull())
	{
		getScript().callMethod("onButtonPress");
	}
}

//------------------------------------------------------------------------------
void Button::onMouseRelease(Event & e)
{
}

//------------------------------------------------------------------------------
void Button::onDrawSelf(DrawBatch & batch)
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

//------------------------------------------------------------------------------
void Button::layoutChildren()
{
    if (hasLayout())
        Control::layoutChildren();
    else
    {
        // Center the first child

        std::vector<Control*> children;
        getChildrenOfType<Control>(children);

        if (!children.empty())
        {
            Control & child = *children[0];
            IntRect childBounds = child.getLocalClientBounds();
            const Anchors & anchors = child.getAnchors();
            const Border & margin = child.getMargin();

            Vector2i pos(0, getPadding().top);

            childBounds.origin() = pos;

            //if (anchors[TGUI_LEFT])
            childBounds.x() = margin.left + getPadding().left;

            if (anchors[TGUI_TOP])
                childBounds.y() = margin.top + getPadding().top;

            if (anchors[TGUI_RIGHT])
                childBounds.width() = getLocalClientBounds().width() - margin.left - margin.right - childBounds.x() - getPadding().right;

            if (anchors[TGUI_BOTTOM])
                childBounds.height() = getLocalClientBounds().height() - margin.top - margin.bottom - childBounds.y() - getPadding().bottom;

            child.setLocalClientBounds(childBounds);
            child.layoutChildren();
        }
    }
}

//------------------------------------------------------------------------------
void Button::serializeState(sn::Variant & o, const SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

}

//------------------------------------------------------------------------------
void Button::unserializeState(const sn::Variant & o, const SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

}

} // namespace tgui



