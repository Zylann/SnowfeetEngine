#include "Control.h"
#include "../GUI.h"

#include <core/util/typecheck.hpp>
#include <functional>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
sn::Vector2i Control::getPosition() const
{
    Control * parent = getParentControl();
    if (parent)
        return getLocalPosition() + parent->getPosition();
    return getLocalPosition();
}

//------------------------------------------------------------------------------
sn::IntRect Control::getClientBounds() const
{
    Vector2i pos = getPosition();
    return IntRect::fromPositionSize(pos.x(), pos.y(), m_localBounds.width(), m_localBounds.height());
}

//------------------------------------------------------------------------------
void Control::setLocalClientBounds(sn::IntRect bounds)
{
    m_localBounds = bounds;
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
    if (parent && parent->isInstanceOf<Control>())
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
void Control::setParent(Entity * newParent)
{
    Control * oldParent = getParentControl();
    Entity::setParent(newParent);
    if (oldParent)
        oldParent->layoutChildren();
    Control * parent = getParentControl();
    if (parent)
        parent->layoutChildren();
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
void Control::layoutChildren()
{
    std::vector<Control*> children;
    getChildrenOfType<Control>(children);

    // Default vertical layout
    Vector2i pos(0, m_padding.top);
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        Control & child = **it;
        if (child.getPositionMode() == TGUI_LAYOUT)
        {
            IntRect childBounds = child.m_localBounds;
            const Anchors & anchors = child.getAnchors();
            const Border & margin = child.getMargin();

            childBounds.origin() = pos;

            //if (anchors[TGUI_LEFT])
            childBounds.x() = margin.left + m_padding.left;

            if (anchors[TGUI_TOP])
                childBounds.y() = margin.top + m_padding.top;

            if (anchors[TGUI_RIGHT])
                childBounds.width() = m_localBounds.width() - margin.left - margin.right - childBounds.x() - m_padding.right;

            if (anchors[TGUI_BOTTOM])
                childBounds.height() = m_localBounds.height() - margin.top - margin.bottom - childBounds.y() - m_padding.bottom;

            child.setLocalClientBounds(childBounds);

            pos.y() += childBounds.height() + margin.top + margin.bottom;
        }
    }
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
void Control::beginCapture()
{
	GUI * gui = getGUI();
	if (gui)
	{
		gui->setCapture(this);
		setControlFlag(TGUI_CF_CAPTURED, true);
	}
}

//------------------------------------------------------------------------------
void Control::endCapture()
{
	GUI * gui = getGUI();
	if (gui)
	{
		SN_ASSERT(gui->getCaptureControl() == this, "Cannot end capture from another control");
		gui->setCapture(nullptr);
		setControlFlag(TGUI_CF_CAPTURED, false);
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
	bool hover = bounds.contains(e.value.mouse.x, e.value.mouse.y);
    if (hover)
    {
        if (!isHovered())
        {
            setControlFlag(TGUI_CF_HOVERED, true);
            onMouseEnter(e);
        }
    }
    else if (isHovered())
    {
        setControlFlag(TGUI_CF_HOVERED, false);
        onMouseLeave(e);
    }
	if (hover || getControlFlag(TGUI_CF_CAPTURED))
	{
		onMouseMove(e);
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
    sn::serialize(o["bounds"], m_localBounds);
    sn::serialize(o["hostWindow"], m_windowID);
    tgui::serialize(o["margins"], m_margins);
    tgui::serialize(o["padding"], m_padding);
    tgui::serializeAnchors(o["anchors"], m_anchors);
}

//------------------------------------------------------------------------------
void Control::unserializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Entity::unserializeState(o, ctx);
    sn::unserialize(o["bounds"], m_localBounds, IntRect(0,0,300,200));
    sn::unserialize(o["hostWindow"], m_windowID);
    tgui::unserialize(o["margins"], m_margins);
    tgui::unserialize(o["padding"], m_padding);
    tgui::unserializeAnchors(o["anchors"], m_anchors);
}

} // namespace tgui


