#include "Button.hpp"
#include "GUI.hpp"
#include "DrawBatch.hpp"

using namespace sn;

namespace tgui
{

void Button::onMousePress(Event & e)
{
    SN_DLOG("Button pressed!");
}

void Button::onMouseRelease(Event & e)
{
    SN_DLOG("Button released!");
}

void Button::onDrawSelf(DrawBatch & batch)
{
    GUI * gui = getGUI();
    if (gui == nullptr)
        return;
    const Theme & theme = gui->getTheme();
    const ControlTheme & ct = theme.controlTheme;

    IntRect bounds = getClientBounds();

    u32 state = ControlTheme::STATE_NORMAL;
    if (isPressed())
        state = ControlTheme::STATE_PRESSED;
    else if (isHovered())
        state = ControlTheme::STATE_HOVERED;

    const IntRect & uvRect = ct.statesUV[state];
    Vector2u ts = theme.getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);
}

void Button::serializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

}

void Button::unserializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

}

} // namespace tgui



