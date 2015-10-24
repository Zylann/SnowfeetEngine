#include <core/util/typecheck.h>

#include "Renderer.h"
#include "GUI.h"

using namespace sn;

namespace tgui
{

void Renderer::onDraw(IDrawContext & dc)
{
    Entity * p = getParent();
    if (p && p->isInstanceOf<GUI>())
    {
        GUI * gui = checked_cast<GUI*>(p);
        gui->draw(dc);
    }
}

} // namespace tgui

