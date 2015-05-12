#include "Slider.h"

using namespace sn;

namespace tgui
{

void serialize(JsonBox::Value & o, Orientation v)
{
    if (v == TGUI_VERTICAL)
        o = "vertical";
    else
        o = "horizontal";
}

void unserialize(JsonBox::Value & o, Orientation v)
{
    const std::string & s = o.getString();
    if (s == "vertical")
        v = TGUI_VERTICAL;
    else
        v = TGUI_HORIZONTAL;
}

void Slider::setValue(f32 v)
{
    m_value = math::clamp(v, 0.f, 1.f);
}

void Slider::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;

    IntRect b = getClientBounds();
    Vector2u ts = theme->getTextureAtlasSize();

    // TODO There is the same code in Button, factorise in ControlTheme?
    u32 state = ControlTheme::STATE_NORMAL;
    if (isPressed())
        state = ControlTheme::STATE_PRESSED;
    else if (isHovered())
        state = ControlTheme::STATE_HOVERED;

    // Bar
    {
        const auto & barTheme = theme->sliderBar;
        const IntRect & barImageRect = barTheme.statesUV[0];
        s32 y = b.minY() + b.height() / 2 - barImageRect.height() / 2;
        batch.fillNineSlices(
            IntRect::fromPositionSize(b.minX(), y, b.width(), barImageRect.height()), 
            barTheme.slicing, barImageRect, ts
        );
    }

    // Thumb
    {
        const auto & thumbTheme = theme->sliderThumbs;
        const IntRect & thumbImageRect = thumbTheme.statesUV[state];
        s32 x = (m_value * static_cast<f32>(b.width() - thumbImageRect.width())) + thumbImageRect.width() / 2;
        s32 y = b.minY() + b.height() / 2 - thumbImageRect.height() / 2;
        batch.fillRect(
            IntRect::fromPositionSize(b.minX() + x, y, thumbImageRect.width(), thumbImageRect.height()),
            thumbImageRect, ts
        );
    }
}

void Slider::onMousePress(Event & ev)
{
    Vector2i mousePos(ev.value.mouse.x, ev.value.mouse.y);
    setValueFromPos(mousePos - getPosition());
    ev.consume();
}

void Slider::onMouseMove(Event & ev)
{
    if (isPressed())
    {
        Vector2i mousePos(ev.value.mouse.x, ev.value.mouse.y);
        setValueFromPos(mousePos - getPosition());
    }
}

void Slider::setValueFromPos(sn::Vector2i cursorPos)
{
    Vector2i size = getSize();
    m_value = static_cast<f32>(cursorPos[m_orientation]) / static_cast<f32>(size[m_orientation]);
}

void Slider::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    tgui::serialize(o, m_orientation);
    sn::serialize(o, m_value);
}

void Slider::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    tgui::unserialize(o, m_orientation);
    sn::unserialize(o, m_value);
}

} // namespace tgui

