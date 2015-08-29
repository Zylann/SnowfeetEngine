#include <core/system/SystemGUI.h>

#include "DockSizer.h"
#include "DockerSplit.h"
#include "Docker.h"

using namespace sn;

namespace tgui
{

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
		Control * parent = getParentControl();
		Vector2i origin = parent ? parent->getPosition() : Vector2i();

		s32 splitPos;
		if (m_orientation == TGUI_HORIZONTAL)
			splitPos = e.value.mouse.x - origin.x();
		else
			splitPos = e.value.mouse.y - origin.y();

		DockerSplit * layout = getDockLayout();
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

DockerSplit * DockSizer::getDockLayout() const
{
	Control * c = getParentControl();
	if (c)
	{
		Docker * docker = Object::cast<Docker>(c);
		if (docker)
		{
			return docker->getDockLayout();
		}
	}
	return nullptr;
}

} // namespace tgui

