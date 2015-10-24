#include "../GUI.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
	namespace
	{
		CURRENT_CLASS(GUI)
	}

	void bindGUI(HSQUIRRELVM vm)
	{
		sn::ScriptableObject::bindBase<GUI>(vm);
	}

} // namespace tgui

