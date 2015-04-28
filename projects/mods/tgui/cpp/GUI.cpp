#include "GUI.hpp"

namespace tgui
{

void GUI::onReady()
{

}

void GUI::onDraw(sn::IDrawContext & dc)
{

}

bool GUI::onSystemEvent(const sn::Event & systemEvent)
{
    tgui::Event ev;
    ev.value = systemEvent;

    dispatchEventToChildren(ev);

    return ev.consumed;
}

} // namespace tgui
