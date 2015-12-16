#include <core/util/Profiler.h>
#include "TabControl.h"
#include "Tab.h"

using namespace sn;

namespace tgui
{

SN_OBJECT_IMPL(TabControl)

//------------------------------------------------------------------------------
TabControl::TabControl() : Control(),
	m_currentPageIndex(INVALID_PAGE),
    m_tabBarSize(24)
{
}

//------------------------------------------------------------------------------
void TabControl::layoutChildren()
{
	layoutTabBar();
	layoutPage();
}

//------------------------------------------------------------------------------
void TabControl::layoutTabBar()
{
	if (!m_tabs.empty())
	{
		// Layout tab bar
		IntRect barBounds = getLocalClientBounds();
		barBounds.x() = 0;
		barBounds.y() = 0;
		barBounds.height() = m_tabBarSize;
		Vector2i pos;
		for (u32 i = 0; i < m_tabs.size(); ++i)
		{
			Control & tab = *m_tabs[i];
			IntRect tabBounds = tab.getLocalClientBounds();
			tabBounds.y() = 0;
			tabBounds.x() = pos.x();
			tabBounds.height() = barBounds.height();
			pos.x() += tabBounds.width();
			tab.setLocalClientBounds(tabBounds);
            tab.layoutChildren();
		}
	}
}

//------------------------------------------------------------------------------
void TabControl::layoutPage()
{
	if (!m_tabs.empty())
	{
		u32 barSize = m_tabs[0]->getSize().y();

		// Layout current page
		u32 pageIndex = getCurrentPageIndex();
		if (pageIndex != INVALID_PAGE)
		{
            if (m_pages[pageIndex])
            {
			    Control & c = *m_pages[pageIndex];
			    IntRect bounds = getLocalClientBounds();
			    bounds.x() = 0;
			    bounds.y() = barSize;
			    bounds.height() -= barSize;
			    c.getMargin().crop(bounds);
			    c.setLocalClientBounds(bounds);
			    c.layoutChildren();
            }
		}
	}
}

//------------------------------------------------------------------------------
void TabControl::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    
    SN_BEGIN_PROFILE_SAMPLE_NAMED("TGUI draw Panel");

    const ControlTheme & ct = theme->panelTheme;

    IntRect bounds = getClientBounds();
    bounds.y() += m_tabBarSize;
    bounds.height() -= m_tabBarSize;

    const IntRect & uvRect = ct.statesUV[0];
    Vector2u ts = theme->getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);

    SN_END_PROFILE_SAMPLE();
}

//------------------------------------------------------------------------------
void TabControl::addPage(Control * page)
{
	if (m_pages.empty())
		m_currentPageIndex = 0;

	m_pages.push_back(page);

	// TODO Allow tab delegates for better customization
	// Default tab control
	Tab * tab = createChild<Tab>();
	m_tabs.push_back(tab);
}

//------------------------------------------------------------------------------
void TabControl::removePage(sn::u32 i)
{
	if (i < m_pages.size())
	{
		u32 currentPage = getCurrentPageIndex();

		m_pages.erase(m_pages.begin() + i);
		Control * tab = m_tabs[i];
		m_tabs.erase(m_tabs.begin() + i);
		tab->destroy();

		if (!m_pages.empty())
		{
			if (currentPage >= m_pages.size())
				currentPage = m_pages.size() - 1;

			setCurrentPageIndex(currentPage);

			layoutChildren();
		}
		else
		{
			m_currentPageIndex = INVALID_PAGE;
		}
	}
}

//------------------------------------------------------------------------------
Control * TabControl::getPage(sn::u32 i)
{
	if (i < m_pages.size())
		return m_pages[i];
	return nullptr;
}

//------------------------------------------------------------------------------
void TabControl::setCurrentPageIndex(sn::u32 i)
{
	u32 currentPage = getCurrentPageIndex();
	if (i != currentPage)
	{
		Control * oldPage = m_pages[currentPage];
		if (oldPage)
			oldPage->setVisible(false);
		
		Control * newPage = m_pages[i];
		if (newPage)
			newPage->setVisible(true);

        m_currentPageIndex = i;

		layoutPage();
	}
}

//------------------------------------------------------------------------------
u32 TabControl::getCurrentPageIndex() const
{
	//for (u32 i = 0; i < m_pages.size(); ++i)
	//{
	//	Control * c = m_pages[i];
	//	if (c && c->isVisible())
	//		return i;
	//}
	//return INVALID_PAGE;
	return m_currentPageIndex;
}

//------------------------------------------------------------------------------
Control * TabControl::getCurrentPage() const
{
	if (m_currentPageIndex != INVALID_PAGE)
		return m_pages[m_currentPageIndex];
	return nullptr;
}

//------------------------------------------------------------------------------
void TabControl::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
	Control::serializeState(o, ctx);
	sn::serialize(o["currentPage"], m_currentPageIndex);

	// TODO Serialize pages
}

//------------------------------------------------------------------------------
void TabControl::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
	Control::unserializeState(o, ctx);

	const Variant::Array & pages = o["pages"].getArray();
	m_pages.reserve(pages.size());
	m_tabs.reserve(pages.size());
	for (u32 i = 0; i < pages.size(); ++i)
	{
		const Variant & page = pages[i];

		// TODO Use ObjectDB references

        u32 tabIndex = page["tab"].getInt();
		Control * tab = getChildControlByIndex(tabIndex);
		if (tab)
		{
            if (tab->isInstanceOf<Tab>())
            {
			    m_tabs.push_back(tab);

                const Variant & pageV = page["page"];
                if (pageV.isInt())
                {
			        Control * child = getChildControlByIndex(page["page"].getInt());
			        m_pages.push_back(child);
                }
                else
                {
                    m_pages.push_back(nullptr);
                }
            }
            else
            {
                SN_ERROR("Child entity " << tabIndex << " " << tab->toString() << " is not a Tab");
            }
		}
	}

    if (!m_tabs.empty())
        m_currentPageIndex = 0;
	sn::unserialize(o["currentPage"], m_currentPageIndex, m_currentPageIndex);
    for (u32 i = 0; i < m_pages.size(); ++i)
    {
        Control * page = m_pages[i];
        if (page)
            page->setVisible(i == m_currentPageIndex);
    }
}

//------------------------------------------------------------------------------
sn::u32 TabControl::getTabIndex(Control * c)
{
	for (u32 i = 0; i < m_tabs.size(); ++i)
	{
		if (m_tabs[i] == c)
			return i;
	}
	return INVALID_PAGE;
}


} // namespace tgui

