#include <core/system/SystemGUI.h>

#include "DockSizer.h"
#include "Docker.h"
#include "../layouts/SplitLayout.h"

using namespace sn;

namespace tgui
{

SN_OBJECT_IMPL(DockSizer)

DockSizer::DockSizer():
	m_orientation(TGUI_VERTICAL)
{
}

void DockSizer::setOrientation(Orientation newOrientation)
{
	m_orientation = newOrientation;
}

void DockSizer::setPath(const DockPath & path)
{
	m_path = path;
}

void DockSizer::onSetCursor(Event & e)
{
	sn::Window * win = SystemGUI::get().getWindowByID(getWindowID());
	if (win)
	{
		if (m_orientation == TGUI_HORIZONTAL)
			win->setMouseCursor(SN_CURSOR_RESIZE_HORIZONTAL);
		else
			win->setMouseCursor(SN_CURSOR_RESIZE_VERTICAL);
		e.consume();
	}
	//else
	//{
	//	Control::onSetCursor(e);
	//}
}

void DockSizer::onMouseMove(Event & e)
{
	if (getControlFlag(TGUI_CF_CAPTURED))
	{
		// Get parent origin
		Control * parent = getParentControl();
		IntRect parentBounds;
		if (parent)
		{
			parentBounds = parent->getClientBounds();
			parent->getPadding().crop(parentBounds);
		}

		// Calculate drag split position
		s32 splitPos;
		if (m_orientation == TGUI_HORIZONTAL)
			splitPos = e.value.mouse.x - parentBounds.minX();
		else
			splitPos = e.value.mouse.y - parentBounds.minY();

		// Set split position
		SplitLayout * layout = getLayout();
		if (layout)
		{
			layout->setSplitPosition(m_path, splitPos);
			parent->layoutChildren();
			//layout->layout(getParentControl()->getLocalClientBounds());
		}
	}
}

void DockSizer::onMousePress(Event & e)
{
	beginCapture();
}

void DockSizer::onMouseRelease(Event & e)
{
	endCapture();
}

SplitLayout * DockSizer::getLayout() const
{
	Control * c = getParentControl();
	if (c)
		return Object::cast<SplitLayout>(c->getLayout());
	return nullptr;
}

} // namespace tgui

