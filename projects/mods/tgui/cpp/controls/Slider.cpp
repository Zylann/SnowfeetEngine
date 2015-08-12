#include <core/util/Profiler.h>
#include "Slider.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
Slider::Slider() : Control(), 
    m_value(0),
    m_range(0, 1),
    m_orientation(TGUI_HORIZONTAL)
{}

//------------------------------------------------------------------------------
void Slider::setValue(f32 v)
{
    v = math::clamp(v, m_range.min(), m_range.max());
    if (v != m_value)
    {
        m_value = v;
        notifyValueChanged();
    }
}

//------------------------------------------------------------------------------
sn::f32 Slider::getValue()
{
    return m_value;
}

//------------------------------------------------------------------------------
void Slider::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;

    SN_BEGIN_PROFILE_SAMPLE_NAMED("TGUI draw Slider");

    IntRect b = getClientBounds();
    Vector2u ts = theme->getTextureAtlasSize();

    // TODO There is the same code in Button, factorise in ControlTheme?
    u32 state = ControlTheme::STATE_NORMAL;
    if (isPressed())
        state = ControlTheme::STATE_PRESSED;
    else if (isHovered())
        state = ControlTheme::STATE_HOVERED;

	// TODO Handle vertical case

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
        f32 value = m_range.inverseLerp(m_value);
        s32 x = b.minX() + static_cast<s32>(value * static_cast<f32>(b.width() - thumbImageRect.width()));
        s32 y = b.minY() + b.height() / 2 - thumbImageRect.height() / 2;
        batch.fillRect(
            IntRect::fromPositionSize(x, y, thumbImageRect.width(), thumbImageRect.height()),
            thumbImageRect, ts
        );
    }

    SN_END_PROFILE_SAMPLE();
}

//------------------------------------------------------------------------------
void Slider::onMousePress(Event & ev)
{
    Vector2i mousePos(ev.value.mouse.x, ev.value.mouse.y);
    setValueFromPos(mousePos - getPosition());
	beginCapture();
    ev.consume();
}

//------------------------------------------------------------------------------
void Slider::onMouseRelease(Event & e)
{
	endCapture();
}

//------------------------------------------------------------------------------
void Slider::onMouseMove(Event & ev)
{
    if (isPressed())
    {
        Vector2i mousePos(ev.value.mouse.x, ev.value.mouse.y);
        setValueFromPos(mousePos - getPosition());
    }
}

//------------------------------------------------------------------------------
f32 Slider::getValueFromPos(sn::Vector2i cursorPos)
{
    Vector2i size = getSize();
	s32 padding = 0;
	const Theme * theme = getTheme();
	if (theme)
	{
		// Apply a padding to the slidable area so we can fix the thumb under the mouse
		padding = theme->sliderThumbs.statesUV[0].size()[m_orientation] / 2;
	}
	f32 rawValue = static_cast<f32>(cursorPos[m_orientation] - padding) / static_cast<f32>(size[m_orientation] - 2 * padding);
    rawValue = math::clamp(rawValue, 0.f, 1.f);
    return m_range.lerp(rawValue);
}

//------------------------------------------------------------------------------
void Slider::setValueFromPos(sn::Vector2i cursorPos)
{
    f32 v = getValueFromPos(cursorPos);
    setValue(v);
}

//------------------------------------------------------------------------------
void Slider::notifyValueChanged()
{
    auto & s = getScript();
    if (!s.isNull())
    {
        s.callMethod("onValueChanged", m_value);
    }
}

//------------------------------------------------------------------------------
void Slider::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    tgui::serialize(o["orientation"], m_orientation);
    sn::serialize(o["value"], m_value);
    sn::serialize(o["range"], m_range);
}

//------------------------------------------------------------------------------
void Slider::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    tgui::unserialize(o["orientation"], m_orientation);
    sn::unserialize(o["value"], m_value);
    sn::unserialize(o["range"], m_range, m_range);

    m_value = math::clamp(m_value, m_range.min(), m_range.max());
}

} // namespace tgui

