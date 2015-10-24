#include "Layout.h"
#include "../controls/Control.h"

using namespace sn;

namespace tgui
{

void Layout::getChildrenToLayout(Control & parent, std::vector<Control*> & out_controls, std::vector<u32> * out_indexes)
{
	for (u32 i = 0; i < parent.getChildCount(); ++i)
	{
		Control * child = Object::cast<Control>(parent.getChildByIndex(i));
		if (child && child->getPositionMode() == TGUI_LAYOUT)
		{
			out_controls.push_back(child);
			if (out_indexes)
				out_indexes->push_back(i);
		}
	}
}

} // namespace tgui

