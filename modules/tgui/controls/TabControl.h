#ifndef __HEADER_TGUI_TABCONTROL__
#define __HEADER_TGUI_TABCONTROL__

#include "Control.h"

namespace tgui
{

class TabControl : public Control
{
public:
	SN_OBJECT(tgui::TabControl, tgui::Control)

	static const sn::u32 INVALID_PAGE = -1;

	TabControl();

	void layoutChildren() override;

	void addPage(Control * page);
	void removePage(sn::u32 i);
	Control * getPage(sn::u32 i);

	void setCurrentPageIndex(sn::u32 i);
	sn::u32 getCurrentPageIndex() const;
	Control * getCurrentPage() const;

	void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
	void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

	// Internal
	sn::u32 getTabIndex(Control * c);

protected:
    void onDrawSelf(DrawBatch & batch) override;

private:
	void layoutTabBar();
	void layoutPage();

private:
	std::vector<Control*> m_pages;
	std::vector<Control*> m_tabs;
	sn::u32 m_currentPageIndex;
    sn::s32 m_tabBarSize;

};

} // namespace tgui

#endif // __HEADER_TGUI_TABCONTROL__

