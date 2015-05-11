#include "Control.h"
#include "GUI.h"

#include <core/util/typecheck.hpp>
#include <functional>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
sn::IntRect Control::getClientBounds() const
{
    return m_clientBounds;
}

//------------------------------------------------------------------------------
void Control::setClientBounds(sn::IntRect bounds)
{
    m_clientBounds = bounds;
}

//------------------------------------------------------------------------------
bool Control::getControlFlag(sn::u32 i) const
{
    return m_controlFlags[i];
}

//------------------------------------------------------------------------------
void Control::setControlFlag(sn::u32 i, bool value)
{
    m_controlFlags[i] = value;
}

//------------------------------------------------------------------------------
Control * Control::getParentControl() const
{
    Entity * parent = getParent();
    if (parent->isInstanceOf<Control>())
        return static_cast<Control*>(parent);
    return nullptr;
}

//------------------------------------------------------------------------------
u32 Control::getWindowID() const
{
    const Control * root = getRootControl();
    if (root)
        return root->getWindowID();
    else
        return m_windowID;
}

//------------------------------------------------------------------------------
const Control * Control::getRootControl() const
{
    const Control * last = nullptr;
    const Control * parent = this;
    while (parent != nullptr)
    {
        if (parent->isInstanceOf<GUI>())
            return last;
        last = parent;
        parent = parent->getParentControl();
    }
    return nullptr;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
const Theme * Control::getTheme() const
{
    GUI * gui = getGUI();
    if (gui == nullptr)
        return nullptr;
    return &(gui->getTheme());
}

//------------------------------------------------------------------------------
Control * Control::getChildControlAt(sn::Vector2i position) const
{
    std::vector<Control*> children;
    getChildrenOfType<Control>(children);
    for (u32 i = 0; i < children.size(); ++i)
    {
        Control & child = *children[i];
        if (child.getClientBounds().contains(position))
        {
            Control * c = child.getChildControlAt(position);
            if (c)
                return c;
            else
                return nullptr;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void Control::onDraw(DrawBatch & batch)
{
    onDrawSelf(batch);

    std::vector<Control*> children;
    getChildrenOfType<Control>(children);

    for (u32 i = 0; i < children.size(); ++i)
    {
        Control & child = *children[i];
        if (child.isEnabled())
        {
            child.onDraw(batch);
        }
    }
}

//------------------------------------------------------------------------------
void Control::onEvent(Event & ev)
{
    dispatchEventToChildren(ev);

    if (!ev.consumed)
    {
        switch (ev.value.type)
        {
        case SN_EVENT_MOUSE_MOVED:
            processMouseMove(ev);
            break;

        case SN_EVENT_MOUSE_DOWN:
            processMousePress(ev);
            break;

        case SN_EVENT_MOUSE_UP:
            processMouseRelease(ev);
            break;

        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------
void Control::dispatchEventToChildren(Event & ev)
{
    std::vector<Control*> children;
    getChildrenOfType<Control>(children);

    for (u32 i = 0; i < children.size(); ++i)
    {
        Control & child = *children[i];
        if (child.isEnabled())
        {
            child.onEvent(ev);
            if (ev.consumed)
                break;
        }
    }
}

//------------------------------------------------------------------------------
void Control::processMouseMove(Event & e)
{
    IntRect bounds = getClientBounds();
    if (bounds.contains(e.value.mouse.x, e.value.mouse.y))
    {
        if (!isHovered())
        {
            setControlFlag(TGUI_CF_HOVERED, true);
            onMouseEnter(e);
        }
        onMouseMove(e);
    }
    else if (isHovered())
    {
        setControlFlag(TGUI_CF_HOVERED, false);
        onMouseLeave(e);
    }
}

//------------------------------------------------------------------------------
void Control::processMousePress(Event & e)
{
    if (isHovered())
    {
        setControlFlag(TGUI_CF_PRESSED, true);
        onMousePress(e);
    }
}

//------------------------------------------------------------------------------
void Control::processMouseRelease(Event & e)
{
    if (isPressed())
    {
        setControlFlag(TGUI_CF_PRESSED, false);
        onMouseRelease(e);
    }
}

//------------------------------------------------------------------------------
void Control::serializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Entity::serializeState(o, ctx);
    sn::serialize(o["bounds"], m_clientBounds);
    sn::serialize(o["hostWindow"], m_windowID);
}

//------------------------------------------------------------------------------
void Control::unserializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Entity::unserializeState(o, ctx);
    sn::unserialize(o["bounds"], m_clientBounds, IntRect(0,0,300,200));
    sn::unserialize(o["hostWindow"], m_windowID);
}

} // namespace tgui


