#ifndef __HEADER_TGUI_DOCKSIZER__
#define __HEADER_TGUI_DOCKSIZER__

#include "Control.h"
#include "../Orientation.h"
#include "../layouts/SplitLayout.h"

namespace tgui
{

class DockSizer : public Control
{
public:
	SN_OBJECT(tgui::DockSizer, tgui::Control)

	DockSizer();

	void setOrientation(Orientation newOrientation);
	Orientation getOrientation() const { return m_orientation; }

	void setPath(const DockPath & path);
	const DockPath & getPath() const { return m_path; }

	// No specific serialization, sizers are usually generated on runtime

protected:
	void onSetCursor(Event & e) override;
	void onMouseMove(Event & e) override;
	void onMousePress(Event & e) override;
	void onMouseRelease(Event & e) override;

	SplitLayout * getLayout() const;

private:
	DockPath m_path;
	Orientation m_orientation;

};

} // namespace tgui

#endif // __HEADER_TGUI_DOCKSIZER__

