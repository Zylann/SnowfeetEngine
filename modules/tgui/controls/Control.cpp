#include "Control.h"
#include "../GUI.h"
#include "../layouts/ListLayout.h"
#include "../layouts/GridLayout.h"
#include "../layouts/SplitLayout.h"

#include <core/util/typecheck.h>
#include <core/system/SystemGUI.h>
#include <functional>

using namespace sn;

namespace tgui
{
   
//------------------------------------------------------------------------------
Control::Control() : sn::Entity(),
    m_controlFlags((1 << TGUI_CF_ENABLED) | (1 << TGUI_CF_VISIBLE)),
    m_windowID(0),
    m_positionMode(TGUI_LAYOUT),
    m_layout(nullptr)
{
}

//------------------------------------------------------------------------------
Control::~Control()
{
    if (m_layout)
    {
        delete m_layout;
    }
}

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
    bool sizeChanged = m_localBounds.size() != bounds.size();
    m_localBounds = bounds;
    if (sizeChanged)
    {
        onSizeChanged();
    }
}

//------------------------------------------------------------------------------
void Control::setAnchors(Anchors anchors)
{
    if (anchors != m_anchors)
    {
        m_anchors = anchors;
    }
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
void Control::setLayout(Layout * newLayout)
{
    if (m_layout)
        delete m_layout;
    m_layout = newLayout;
	m_layout->setContainer(*this);
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
        return root->m_windowID;
    else
        return m_windowID;
    //const Control * root = getRootControl();
    //if (root)
    //    return root->getWindowID();
    //else
    //    return m_windowID;
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
void Control::onReady()
{
	if (m_layout)
		m_layout->onReady();
	layoutChildren();
	Entity::onReady();
}

//------------------------------------------------------------------------------
void Control::onDestroy()
{
	Control * parent = getParentControl();
	if (parent)
		parent->onChildControlRemoved(*this);
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
Control * Control::getChildControlByIndex(u32 i) const
{
    Entity * e = getChildByIndex(i);
    Control * c = Object::cast<Control>(e);
    return c;
}

//------------------------------------------------------------------------------
void Control::layoutChildren()
{
    if (m_layout)
    {
        m_layout->update();
    }
	else
	{
		// Default layout: only anchors are applied

		std::vector<Control*> children;
		getChildrenOfType<Control>(children);

		IntRect cellBounds = getLocalClientBounds();
		cellBounds.x() = 0;
		cellBounds.y() = 0;
		getPadding().crop(cellBounds);

		for (auto it = children.begin(); it != children.end(); ++it)
		{
			Control & child = **it;
			IntRect childBounds = child.getLocalClientBounds();
			applyAnchors(childBounds, cellBounds, child.getAnchors());
			child.setLocalClientBounds(childBounds);
			child.layoutChildren();
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
        if (child.isVisible())
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
void Control::setFocus(bool isFocused)
{
    GUI * gui = getGUI();
    if (gui)
    {
        if (isFocused)
        {
            Control * oldFocus = gui->getFocusedControl();
            if (oldFocus)
                oldFocus->setControlFlag(TGUI_CF_FOCUSED, false);
            gui->setFocusedControl(this);
        }
        setControlFlag(TGUI_CF_FOCUSED, isFocused);
    }
}

//------------------------------------------------------------------------------
void Control::setVisible(bool visible)
{
    setControlFlag(TGUI_CF_VISIBLE, visible);
}

//------------------------------------------------------------------------------
void Control::onEvent(Event & ev)
{
    // If events are not exclusive to the control, forward to children first
    if (!getControlFlag(TGUI_CF_CAPTURED))
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

        case SN_EVENT_KEY_DOWN:
            processKeyDown(ev);
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
        if (child.isEnabled() && child.isVisible())
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
            onSetCursor(e);
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
        onSetCursor(e);
	}
}

//------------------------------------------------------------------------------
void Control::processMousePress(Event & e)
{
    if (isHovered())
    {
        setControlFlag(TGUI_CF_PRESSED, true);
        onMousePress(e);
        setFocus(true);
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
void Control::processKeyDown(Event & e)
{
    if (isFocused())
    {
        onKeyDown(e);
    }
}

//------------------------------------------------------------------------------
void Control::onSetCursor(Event & e)
{
    sn::Window * win = SystemGUI::get().getWindowByID(getWindowID());
    if (win)
        win->setMouseCursor(SN_CURSOR_DEFAULT);
}

//------------------------------------------------------------------------------
void Control::onChildControlRemoved(Control & child)
{
	if (m_layout)
	{
		m_layout->onControlRemoved(child);
	}
}

//------------------------------------------------------------------------------
void Control::serializeState(sn::Variant & o, const SerializationContext & ctx)
{
    Entity::serializeState(o, ctx);
    // TODO Convert to Variant
    //sn::serialize(o["bounds"], m_localBounds);
    //sn::serialize(o["hostWindow"], m_windowID);
    //tgui::serialize(o["margins"], m_margins);
    //tgui::serialize(o["padding"], m_padding);
    //tgui::serializeAnchors(o["anchors"], m_anchors);
}

//------------------------------------------------------------------------------
void Control::unserializeState(const sn::Variant & o, const SerializationContext & ctx)
{
    Entity::unserializeState(o, ctx);
    sn::unserialize(o["bounds"], m_localBounds, IntRect(0,0,100,50));
    sn::unserialize(o["hostWindow"], m_windowID);
    tgui::unserialize(o["margins"], m_margins);
    tgui::unserialize(o["padding"], m_padding);
    tgui::unserializeAnchors(o["anchors"], m_anchors);

    const Variant & layoutData = o["layout"];
    if (layoutData.isDictionary())
    {
        std::string layoutType;
        sn::unserialize(layoutData["@type"], layoutType);

        if (!layoutType.empty())
        {
			Layout * layout = static_cast<Layout*>(instantiateDerivedObject(layoutType, sn::getObjectType<Layout>()));
			if (layout)
			{
				setLayout(layout);
				layout->unserializeState(layoutData, ctx);
			}
			else
			{
				SN_ERROR("Unrecognized TGUI layout type '" << layoutType << "'");
			}
        }
    }
}

} // namespace tgui


