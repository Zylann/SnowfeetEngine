#ifndef __HEADER_TGUI_SLIDER__
#define __HEADER_TGUI_SLIDER__

#include "Control.h"
#include "../DrawBatch.h"
#include "../Orientation.h"

namespace tgui
{

class Slider : public Control
{
public:
    SN_ENTITY(tgui::Slider, tgui::Control)

    Slider() : Control(), 
        m_value(0),
        m_orientation(TGUI_HORIZONTAL)
    {}

    void setValue(sn::f32 v);

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch);

    void onMousePress(Event & ev) override;
    void onMouseMove(Event & ev) override;
	void onMouseRelease(Event & ev) override;

private:
    void setValueFromPos(sn::Vector2i cursorPos);

private:
    // TODO Step?
    sn::f32 m_value;
    Orientation m_orientation;

};

} // namespace tgui

#endif // __HEADER_TGUI_SLIDER__


