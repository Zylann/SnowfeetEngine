#include "DockerSplit.h"
#include "Docker.h"
#include "DockSizer.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
DockerSplit::DockerSplit(DockerSplit * parent, Control * c):
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
DockerSplit::~DockerSplit()
{
    clearChildren();
}
    
//------------------------------------------------------------------------------
bool DockerSplit::isLeaf() const
{
    return m_children[0] == nullptr 
        && m_children[1] == nullptr;
}
    
//------------------------------------------------------------------------------
bool DockerSplit::isRoot() const
{
    return r_parent == nullptr;
}
    
//------------------------------------------------------------------------------
DockPath DockerSplit::getPath(const DockerSplit ** root) const
{
    DockPath path;
    if(r_parent == nullptr)
        return path;
    const DockerSplit * parent = this;
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
DockerSplit * DockerSplit::getFromPath(const DockPath & path)
{
    DockerSplit * s = this;
    for(size_t j = 0; j < path.size() && s != nullptr; ++j)
    {
        u8 i = path[j];
        s = s->m_children[i];
    }
    return s;
}

//------------------------------------------------------------------------------
u8 DockerSplit::getIndex() const
{
    if(r_parent == nullptr)
        return -1;
    auto & parent = *r_parent;
    if(r_parent->m_children[0] == this)
        return 0;
    return 1;
}
    
//------------------------------------------------------------------------------
void DockerSplit::split(Control * dockContent, Direction direction)
{
    SN_ASSERT(isLeaf(), "Cannot split non-leaf splitter!");
    
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

    m_children[a] = new DockerSplit(this, r_control);
    m_children[b] = new DockerSplit(this, dockContent);
    m_orientation = o;
    m_position = 0.5f;
    r_control = nullptr;
}

//------------------------------------------------------------------------------
void DockerSplit::join(u8 i)
{
    SN_ASSERT(!isLeaf(), "Cannot join leaf splitter!");
        
    r_control = m_children[i]->r_control;
    clearChildren();
}

//------------------------------------------------------------------------------
void DockerSplit::setLeaf(Control * c)
{
    SN_ASSERT(isLeaf(), "Cannot set control of non-leaf splitter!");
    r_control = c;
}

//------------------------------------------------------------------------------
Vector2i DockerSplit::getSize() const
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
void DockerSplit::setSplitPosition(const DockPath & path, sn::s32 pixelPos)
{
	// Get target split
	DockerSplit * target = getFromPath(path);
	SN_ASSERT(target != nullptr, "Invalid path was provided");

	// Calculate offset from the root
	DockerSplit * split = this;
	s32 offset = 0;
	for (u32 i = 0; i < path.size(); ++i)
	{
		u8 childIndex = path[i];
		DockerSplit * parent = split;
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
void DockerSplit::setLocalSplitPosition(sn::s32 pixelPos)
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
sn::s32 DockerSplit::getLocalSplitPosition() const
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
void DockerSplit::clearChildren()
{
    for (u32 i = 0; i < 2; ++i)
    {
        if (m_children[0])
        {
            delete m_children[0];
            m_children[i] = nullptr;
        }
    }
}

//------------------------------------------------------------------------------
void DockerSplit::layout(const IntRect & bounds)
{
    if (isLeaf())
    {
        if (r_control)
        {
            r_control->setLocalClientBounds(bounds);
        }
    }
    else
    {
        SN_ASSERT(m_children[0] && m_children[1], "Invalid state");

        DockerSplit & a = *m_children[0];
        DockerSplit & b = *m_children[1];

        // TODO Apply margins
        // TODO Apply docker padding
        // TODO Apply splitters size

		s32 sizerSize = 4;

        if (m_orientation == TGUI_HORIZONTAL)
        {
			s32 localSplitPos = bounds.minX() + static_cast<s32>(m_position * static_cast<f32>(bounds.width()));
			if (r_sizer)
				r_sizer->setLocalClientBounds(IntRect::fromPositionSize(
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
				r_sizer->setLocalClientBounds(IntRect::fromPositionSize(
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
void DockerSplit::createSizers(Control & container)
{
	if (!isLeaf() && r_sizer == nullptr)
	{
		r_sizer = container.createChild<DockSizer>();
		r_sizer->setPath(getPath());
		r_sizer->setOrientation(m_orientation);

		for (u32 i = 0; i < 2; ++i)
		{
			DockerSplit * child = m_children[i];
			if (child)
				child->createSizers(container);
		}
	}
}

//------------------------------------------------------------------------------
void DockerSplit::serializeTree(Variant & o, const SerializationContext & ctx)
{
    SN_ERROR("DockerSplit::serializeTree is not implemented yet");
    // TODO

}

//------------------------------------------------------------------------------
void DockerSplit::unserializeTree(const Docker & docker, const Variant & o, const SerializationContext & ctx)
{
    if (o.isInt())
    {
        // Only one dock
        Entity * e = docker.getChildByIndex(o.getInt());
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
                        m_children[i] = new DockerSplit(this);
                        m_children[i]->unserializeTree(docker, splitArray[i+2], ctx);
                    }

                    // TODO Serialize splitter position as well
                    //if (m_orientation == TGUI_HORIZONTAL)
                    //    m_position = m_children[0]->getSize().x() / 2;
                    //else
                    //    m_position = m_children[0]->getSize().y() / 2;
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

