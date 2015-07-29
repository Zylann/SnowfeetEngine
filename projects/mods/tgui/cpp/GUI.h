#ifndef __HEADER_TGUI_GUI__
#define __HEADER_TGUI_GUI__

#include <core/scene/Drawable.h>
#include <core/scene/base/IDrawContext.h>
#include <core/util/SharedRef.h>
#include <core/system/Event.h>

#include "controls/Control.h"
#include "theme/Theme.h"

namespace tgui
{

class GUI : public Control
{
public:
    SN_ENTITY(tgui::GUI, sn::Drawable)

    GUI();

    void draw(sn::IDrawContext & dc);

    void onReady() override;

    const Theme & getTheme() const;

	void setCapture(Control * captureControl);
	const Control * getCaptureControl() const { return r_captureControl; }

    void setFocusedControl(Control * focusControl);
    Control * getFocusedControl() const { return r_focusControl; }

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    ~GUI();

    bool onSystemEvent(const sn::Event & systemEvent) override;

private:
    Theme * m_defaultTheme;
	Control * r_captureControl;
    Control * r_focusControl;
    sn::SharedRef<Theme> m_theme;
    std::bitset<sn::SN_EVENT_COUNT> m_eventFilter;

};

} // namespace tgui

#endif // __HEADER_TGUI_GUI__

