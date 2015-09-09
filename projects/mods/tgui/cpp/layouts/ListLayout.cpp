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
    
	// Get container bounds with padding
	sn::IntRect localBounds = container->getLocalClientBounds();
	localBounds.x() = 0;
	localBounds.y() = 0;
	container->getPadding().crop(localBounds);

	IntRect cellBounds;
	cellBounds.origin() = localBounds.origin();
	cellBounds.width() = localBounds.width();

	for (auto it = children.begin(); it != children.end(); ++it)
    {
        Control & child = **it;
        if (child.getPositionMode() == TGUI_LAYOUT)
        {
			const Border & margin = child.getMargin();
            IntRect childBounds = child.getLocalClientBounds();

			childBounds.origin() = cellBounds.origin();

			// Anchors
			cellBounds.height() = childBounds.height() + margin.top + margin.bottom;
			applyAnchors(childBounds, cellBounds, child.getAnchors());

			// TODO lastChildFill property

			// Margin
			child.getMargin().crop(childBounds);

			// Apply
            child.setLocalClientBounds(childBounds);
            child.layoutChildren();

			cellBounds.y() += cellBounds.height() + m_spacing;
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

