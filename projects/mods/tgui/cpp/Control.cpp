#include "Control.hpp"
#include <core/util/typecheck.hpp>
#include <functional>

using namespace sn;

namespace tgui
{

sn::IntRect Control::getClientBounds() const
{
    return m_clientBounds;
}

void Control::setClientBounds(sn::IntRect bounds)
{
    m_clientBounds = bounds;
}

bool Control::getControlFlag(sn::u32 i)
{
    return m_controlFlags[i];
}

void Control::setControlFlag(sn::u32 i, bool value)
{
    m_controlFlags[i] = value;
}

Control * Control::getParentControl() const
{
    Entity * parent = getParent();
    if (parent->isInstanceOf<Control>())
        return static_cast<Control*>(parent);
    return nullptr;
}

GUI * Control::getGUI() const
{
    Entity * parent = getParent();
    while (parent != nullptr)
    {
        if (parent->isInstanceOf<GUI>())
            return checked_cast<GUI*>(parent);
        parent = parent->getParent();
    }
    return nullptr;
}

void Control::onEvent(Event & ev)
{
    switch (ev.value.type)
    {
    case SN_EVENT_MOUSE_MOVED:
        onMouseMove(ev);
        break;
    }

    if (!ev.consumed)
    {
        dispatchEventToChildren(ev);
    }
}

void Control::dispatchEventToChildren(Event & ev)
{
    for (u32 i = 0; i < getChildCount(); ++i)
    {
        Entity & e = *getChildByIndex(i);
        if (e.isEnabled() && e.isInstanceOf<Control>())
        {
            Control & child = *checked_cast<Control*>(&e);
            child.onEvent(ev);
            if (ev.consumed)
                break;
        }
    }
}

} // namespace tgui


