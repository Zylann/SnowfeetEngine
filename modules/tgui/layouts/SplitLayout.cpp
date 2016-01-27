#include "SplitLayout.h"
#include "../controls/DockSizer.h"

using namespace sn;

namespace tgui
{

SN_OBJECT_IMPL(SplitLayout)

//------------------------------------------------------------------------------
SplitLayout::SplitLayout(Control * container, SplitLayout * parent, Control * c) :
	Layout(container),
    m_position(0.5f),
    m_orientation(TGUI_HORIZONTAL),
    r_control(c),
	r_sizer(nullptr),
    r_parent(parent)
{
    m_children[0] = nullptr;
    m_children[1] = nullptr;
}
    
//------------------------------------------------------------------------------
SplitLayout::~SplitLayout()
{
    clearChildren();
}
    
//------------------------------------------------------------------------------
bool SplitLayout::isLeaf() const
{
    return m_children[0] == nullptr 
        && m_children[1] == nullptr;
}
    
//------------------------------------------------------------------------------
bool SplitLayout::isRoot() const
{
    return r_parent == nullptr;
}
    
//------------------------------------------------------------------------------
DockPath SplitLayout::getPath(const SplitLayout ** root) const
{
    DockPath path;
    if(r_parent == nullptr)
        return path;
    const SplitLayout * parent = this;
    while(parent->r_parent)
    {
        path.push_back(parent->getIndex());
		if (root)
			*root = parent;
        parent = parent->r_parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
    
//------------------------------------------------------------------------------
SplitLayout * SplitLayout::getFromPath(const DockPath & path)
{
    SplitLayout * s = this;
    for(size_t j = 0; j < path.size() && s != nullptr; ++j)
    {
        u8 i = path[j];
        s = s->m_children[i];
    }
    return s;
}

//------------------------------------------------------------------------------
u8 SplitLayout::getIndex() const
{
    if(r_parent == nullptr)
        return -1;
    auto & parent = *r_parent;
    if(r_parent->m_children[0] == this)
        return 0;
    return 1;
}
    
//------------------------------------------------------------------------------
void SplitLayout::split(Control * dockContent, Direction direction)
{
    SN_ASSERT(isLeaf(), "Cannot split non-leaf splitter!");
	Control * container = getContainer();
	SN_ASSERT(container != nullptr, "Layout container is null!");
    
    u32 a, b;
    Orientation o;
    
    switch(direction)
    {
    case TGUI_RIGHT:
        o = TGUI_HORIZONTAL;
        a = 0;
        b = 1;
        break;
    case TGUI_LEFT:
        o = TGUI_HORIZONTAL;
        a = 1;
        b = 0;
        break;
    case TGUI_BOTTOM:
        o = TGUI_VERTICAL;
        a = 0;
        b = 1;
        break;
    case TGUI_TOP:
        o = TGUI_VERTICAL;
        a = 1;
        b = 0;
        break;
    }

    m_children[a] = new SplitLayout(container, this, r_control);
    m_children[b] = new SplitLayout(container, this, dockContent);
    m_orientation = o;
    m_position = 0.5f;
    r_control = nullptr;
}

//------------------------------------------------------------------------------
void SplitLayout::join(u8 i)
{
    SN_ASSERT(!isLeaf(), "Cannot join leaf splitter!");
        
    r_control = m_children[i]->r_control;
    clearChildren();
}

//------------------------------------------------------------------------------
void SplitLayout::setLeaf(Control * c)
{
    SN_ASSERT(isLeaf(), "Cannot set control of non-leaf splitter!");
    r_control = c;
}

//------------------------------------------------------------------------------
Vector2i SplitLayout::getSize() const
{
    if (isLeaf())
        return r_control->getSize();
    else
    {
        Vector2i s = m_children[0]->getSize();
        if (m_orientation == TGUI_HORIZONTAL)
        {
            s.x() += m_children[1]->getSize().x();
            if (r_sizer)
                s.x() += r_sizer->getSize().x();
        }
        else
        {
            s.y() += m_children[1]->getSize().y();
            if (r_sizer)
                s.y() += r_sizer->getSize().y();
        }
        return s;
    }
}

//------------------------------------------------------------------------------
void SplitLayout::setSplitPosition(const DockPath & path, sn::s32 pixelPos)
{
	// Get target split
	SplitLayout * target = getFromPath(path);
	SN_ASSERT(target != nullptr, "Invalid path was provided");

	// Calculate offset from the root
	SplitLayout * split = this;
	s32 offset = 0;
	for (u32 i = 0; i < path.size(); ++i)
	{
		u8 childIndex = path[i];
		SplitLayout * parent = split;
		split = split->m_children[childIndex];
		// TODO This might be biased by margins
		// If the child is placed at +X or +Y and is oriented the same, add offset
		if (childIndex != 0 && split->m_orientation == parent->m_orientation)
		{
			offset += parent->getLocalSplitPosition();
		}
	}

	// Set sizer position
	target->setLocalSplitPosition(pixelPos - offset);
}

//------------------------------------------------------------------------------
void SplitLayout::setLocalSplitPosition(sn::s32 pixelPos)
{
	Vector2i size = getSize();
	if (m_orientation == TGUI_HORIZONTAL)
	{
		if (size.x() != 0)
			m_position = static_cast<f32>(pixelPos) / static_cast<f32>(size.x());
	}
	else
	{
		if (size.y() != 0)
			m_position = static_cast<f32>(pixelPos) / static_cast<f32>(size.y());
	}
}

//------------------------------------------------------------------------------
sn::s32 SplitLayout::getLocalSplitPosition() const
{
	Vector2i size = getSize();
	if (m_orientation == TGUI_HORIZONTAL)
	{
		return static_cast<s32>(static_cast<f32>(size.x()) * m_position);
	}
	else
	{
		return static_cast<s32>(static_cast<f32>(size.y()) * m_position);
	}
}

//------------------------------------------------------------------------------
void SplitLayout::clearChildren()
{
    for (u32 i = 0; i < 2; ++i)
    {
        if (m_children[i])
        {
            delete m_children[i];
            m_children[i] = nullptr;
        }
    }
}

//------------------------------------------------------------------------------
void SplitLayout::update()
{
	Control * container = getContainer();
	SN_ASSERT(container != nullptr, "SplitLayout container is null!");
	IntRect bounds = container->getLocalBounds();
	bounds.x() = 0;
	bounds.y() = 0;
	container->getPadding().crop(bounds);
	layout(bounds);
}

//------------------------------------------------------------------------------
void SplitLayout::onReady()
{
	Control * container = getContainer();
	SN_ASSERT(container != nullptr, "SplitLayout container is null!");
	createSizers(*container);
}

//------------------------------------------------------------------------------
void SplitLayout::onControlRemoved(const Control & c)
{
	if (r_control == &c)
		r_control = nullptr;
	else if (r_sizer == &c)
		r_sizer = nullptr;
	else
	{
		if (!isLeaf())
		{
			for (u32 i = 0; i < 2; ++i)
				m_children[i]->onControlRemoved(c);
		}
	}
}

//------------------------------------------------------------------------------
void SplitLayout::layout(const IntRect & bounds)
{
    if (isLeaf())
    {
        if (r_control)
        {
            r_control->setLocalBounds(bounds);
			r_control->layoutChildren();
        }
    }
    else
    {
        SN_ASSERT(m_children[0] && m_children[1], "Invalid state");

        SplitLayout & a = *m_children[0];
        SplitLayout & b = *m_children[1];

        // TODO Apply margins
        // TODO Clear duplication with array component access

		s32 sizerSize = 4;

        if (m_orientation == TGUI_HORIZONTAL)
        {
			s32 localSplitPos = static_cast<s32>(m_position * static_cast<f32>(bounds.width()));
			if (r_sizer)
				r_sizer->setLocalBounds(IntRect::fromPositionSize(
					bounds.minX() + localSplitPos - sizerSize / 2,
					bounds.minY(), 
					sizerSize, 
					bounds.height()
				));
			else
				sizerSize = 0;
			a.layout(IntRect::fromPositionSize(
				bounds.minX(), 
				bounds.minY(), 
				localSplitPos - sizerSize / 2,
				bounds.height()
			));
            b.layout(IntRect::fromPositionSize(
				bounds.minX() + localSplitPos + sizerSize / 2,
				bounds.minY(), 
				bounds.width() - localSplitPos - sizerSize / 2,
				bounds.height()
			));
        }
        else
        {
			s32 localSplitPos = static_cast<s32>(m_position * static_cast<f32>(bounds.height()));
			if (r_sizer)
				r_sizer->setLocalBounds(IntRect::fromPositionSize(
					bounds.minX(), 
					bounds.minY() + localSplitPos - sizerSize / 2,
					bounds.width(), 
					sizerSize
				));
			else
				sizerSize = 0;
			a.layout(IntRect::fromPositionSize(
				bounds.minX(), 
				bounds.minY(), 
				bounds.width(), 
				localSplitPos - sizerSize / 2
			));
			b.layout(IntRect::fromPositionSize(
				bounds.minX(), 
				bounds.minY() + localSplitPos + sizerSize / 2,
				bounds.width(), 
				bounds.height() - localSplitPos - sizerSize / 2
			));
        }
    }
}

//------------------------------------------------------------------------------
void SplitLayout::createSizers(Control & container)
{
	// TODO FIXME Some sizers don't appear before the first call to layout() (but they exist)
	if (!isLeaf() && r_sizer == nullptr)
	{
		r_sizer = container.createChild<DockSizer>();
		r_sizer->setPath(getPath());
		r_sizer->setOrientation(m_orientation);

		for (u32 i = 0; i < 2; ++i)
		{
			SplitLayout * child = m_children[i];
			if (child)
				child->createSizers(container);
		}
	}
}

//------------------------------------------------------------------------------
void SplitLayout::serializeState(Variant & o, const SerializationContext & ctx)
{
	Layout::serializeState(o, ctx);
	serializeTree(o["split"], ctx);
}

//------------------------------------------------------------------------------
void SplitLayout::unserializeState(const Variant & o, const SerializationContext & ctx)
{
	Control * container = getContainer();
	SN_ASSERT(container != nullptr, "SplitLayout container is null!");

	unserializeTree(*container, o["split"], ctx);
}

//------------------------------------------------------------------------------
void SplitLayout::serializeTree(Variant & o, const SerializationContext & ctx)
{
    SN_ERROR("SplitLayout::serializeTree is not implemented yet");
    // TODO

}

//------------------------------------------------------------------------------
void SplitLayout::unserializeTree(Control & container, const Variant & o, const SerializationContext & ctx)
{
	if (o.isInt())
    {
        // No split, the control takes the entire space
		Entity * e = container.getChildByIndex(o.getInt());
        Control * c = Object::cast<Control>(e);
        if (c)
        {
            setLeaf(c);
        }
    }
	else if (o.isArray())
    {
		const Variant::Array & splitArray = o.getArray();
        if (splitArray.size() >= 1)
        {
            // Split type
            const Variant::String & splitType = splitArray[0].getString();
            Orientation splitDir;
            bool isTab = false;
            if (splitType == "h")
                splitDir = TGUI_HORIZONTAL;
            else if (splitType == "v")
                splitDir = TGUI_VERTICAL;
            else
                isTab = true;

            if (isTab)
            {
                // Tabbed layout

                SN_ERROR("Tabbed docks is not implemented yet");
                // TODO Tabs support

            }
            else
            {
                // Split layout

                r_control = nullptr;
                m_orientation = splitDir;
                clearChildren();

                if (splitArray.size() == 4)
                {
					sn::unserialize(splitArray[1], m_position, m_position);

                    for (u32 i = 0; i < 2; ++i)
                    {
						m_children[i] = new SplitLayout(&container, this);
                        m_children[i]->unserializeTree(container, splitArray[i+2], ctx);
                    }
                }
                else
                {
                    SN_ERROR("Expected [type,position,first,second] for split dock layout");
                }
            }
        }
        else
        {
            SN_ERROR("Expected non-empty array for split dock layout");
        }
    }
}


} // namespace tgui

