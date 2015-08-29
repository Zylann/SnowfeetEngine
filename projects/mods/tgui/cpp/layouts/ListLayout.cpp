#include "ListLayout.h"
#include "../controls/Control.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
ListLayout::ListLayout(Control * control):
    Layout(control),
    m_spacing(0),
    m_orientation(TGUI_VERTICAL)
{
}

//------------------------------------------------------------------------------
void ListLayout::setOrientation(Orientation newOrientation)
{
    m_orientation = newOrientation;
}

//------------------------------------------------------------------------------
void ListLayout::setSpacing(sn::s32 newSpacing)
{
    m_spacing = newSpacing;
}

//------------------------------------------------------------------------------
void ListLayout::update()
{
	Control * container = getContainer();
	SN_ASSERT(container != nullptr, "ListLayout container is null!");

    if (m_orientation == TGUI_HORIZONTAL)
    {
        SN_WARNING("Vertical ListLayout orientation is not implemented yet");
        return;
    }

	// TODO Have a dirty flag to avoid calculating layouts when nothing changed

    std::vector<Control*> children;
	container->getChildrenOfType<Control>(children);

    // Default vertical layout
    
	const Border & padding = container->getPadding();
    Vector2i pos(0, padding.top);

	const sn::IntRect & localBounds = container->getLocalClientBounds();

    for (auto it = children.begin(); it != children.end(); ++it)
    {
        Control & child = **it;
        if (child.getPositionMode() == TGUI_LAYOUT)
        {
            IntRect childBounds = child.getLocalClientBounds();
            const Anchors & anchors = child.getAnchors();
            const Border & margin = child.getMargin();

            childBounds.origin() = pos;

            //if (anchors[TGUI_LEFT])
            childBounds.x() = margin.left + padding.left;

            if (anchors[TGUI_TOP])
                childBounds.y() = margin.top + padding.top;

            if (anchors[TGUI_RIGHT])
                childBounds.width() = localBounds.width() - margin.left - margin.right - childBounds.x() - padding.right;

            if (anchors[TGUI_BOTTOM])
                childBounds.height() = localBounds.height() - margin.top - margin.bottom - childBounds.y() - padding.bottom;

            child.setLocalClientBounds(childBounds);
            child.layoutChildren();

            pos.y() += childBounds.height() + margin.top + margin.bottom + m_spacing;
        }
    }
}

//------------------------------------------------------------------------------
void ListLayout::serializeState(Variant & o, const SerializationContext & ctx)
{
    Layout::serializeState(o, ctx);

    sn::serialize(o["spacing"], m_spacing);
    tgui::serialize(o["orientation"], m_orientation);
}

//------------------------------------------------------------------------------
void ListLayout::unserializeState(const Variant & o, const SerializationContext & ctx)
{
    Layout::unserializeState(o, ctx);

    sn::unserialize(o["spacing"], m_spacing);
    tgui::unserialize(o["orientation"], m_orientation);
}

} // namespace tgui

