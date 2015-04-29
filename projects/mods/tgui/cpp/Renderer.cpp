#include <core/util/typecheck.hpp>

#include "Renderer.hpp"
#include "GUI.hpp"

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

