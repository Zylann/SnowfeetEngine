#ifndef __HEADER_TGUI_SLIDER__
#define __HEADER_TGUI_SLIDER__

#include "Control.h"
#include "../DrawBatch.h"
#include "../Orientation.h"

namespace tgui
{

/// \brief Linear widget with a thumb that can be dragged between two ends.
class Slider : public Control
{
public:
    SN_OBJECT(tgui::Slider, tgui::Control)

    Slider();

    void setValue(sn::f32 v);
    sn::f32 getValue();

    void setStep(sn::f32 step);
    sn::f32 getStep() const { return m_step; }

    void setMin(sn::f32 step);
    void setMax(sn::f32 max);

    sn::f32 getMin() const { return m_range.min(); }
    sn::f32 getMax() const { return m_range.max(); }

    void setStepEnabled(bool enable);
    bool isStepEnabled() const { return m_stepEnabled; }

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
    void onDrawSelf(DrawBatch & batch);

    void onMousePress(Event & ev) override;
    void onMouseMove(Event & ev) override;
	void onMouseRelease(Event & ev) override;

private:
    sn::f32 getValueFromPos(sn::Vector2i cursorPos);
    void setValueFromPos(sn::Vector2i cursorPos);

    void notifyValueChanged();
    void notifyReleased();

private:
    // TODO Step?
	// TODO Reset to default value on double-click?
    sn::f32 m_value;
    sn::Range<sn::f32> m_range;
    sn::f32 m_step;
    bool m_stepEnabled;
    Orientation m_orientation;

};

} // namespace tgui

#endif // __HEADER_TGUI_SLIDER__


