#ifndef __HEADER_TGUI_TAB__
#define __HEADER_TGUI_TAB__

#include <modules/tgui/controls/Control.h>

namespace tgui
{

/// \brief Specialized control for tab buttons in a TabControl
class SN_TGUI_API Tab : public Control
{
public:
	SN_OBJECT

	void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
	void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
	void onMouseMove(Event & e);
	void onMousePress(Event & e);
	void onMouseRelease(Event & e);

    void onDrawSelf(DrawBatch & batch);

};

} // namespace tgui

#endif // __HEADER_TGUI_TAB__

