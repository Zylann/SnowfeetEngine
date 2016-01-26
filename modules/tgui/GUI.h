#ifndef __HEADER_TGUI_GUI__
#define __HEADER_TGUI_GUI__

#include <core/util/SharedRef.h>
#include <core/system/Event.h>

#include <modules/render/VideoDriver.h>

#include "controls/Control.h"
#include "theme/Theme.h"

namespace tgui
{

class GUI : public Control
{
public:
    SN_OBJECT

    GUI();

    void draw(sn::VideoDriver & driver);

    void onReady() override;

    const Theme & getTheme() const;

	void beginCapture(Control & captureControl, const Event & event);
    void endCapture();
	const Control * getCaptureControl() const { return r_captureControl; }
    bool isDragThresholdReached() const { return m_dragThresholdReached; }
    const sn::Vector2i getCaptureOrigin() const { return m_beginCaptureMousePos; }
    const sn::Vector2i getCaptureControlOrigin() const { return m_beginCaptureControlPos; }

    void setFocusedControl(Control * focusControl);
    Control * getFocusedControl() const { return r_focusControl; }

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
    ~GUI();

    bool onSystemEvent(const sn::Event & systemEvent) override;

private:
    Theme * m_defaultTheme;
	Control * r_captureControl;
    Control * r_focusControl;
    bool m_dragThresholdReached;
    sn::Vector2i m_beginCaptureMousePos;
    sn::Vector2i m_beginCaptureControlPos;
    sn::SharedRef<Theme> m_theme;
    std::bitset<sn::SN_EVENT_COUNT> m_eventFilter;
    sn::ClearMask m_clearMask;
    sn::Color m_clearColor;

};

} // namespace tgui

#endif // __HEADER_TGUI_GUI__

