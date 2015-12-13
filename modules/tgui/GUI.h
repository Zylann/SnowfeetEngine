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
    SN_OBJECT(tgui::GUI, tgui::Control)

    GUI();

    void draw(sn::VideoDriver & driver);

    void onReady() override;

    const Theme & getTheme() const;

	void setCapture(Control * captureControl);
	const Control * getCaptureControl() const { return r_captureControl; }

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
    sn::SharedRef<Theme> m_theme;
    std::bitset<sn::SN_EVENT_COUNT> m_eventFilter;
    sn::ClearMask m_clearMask;
    sn::Color m_clearColor;

};

} // namespace tgui

#endif // __HEADER_TGUI_GUI__

