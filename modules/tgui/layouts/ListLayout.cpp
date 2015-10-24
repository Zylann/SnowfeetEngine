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

	// TODO Have a dirty flag to avoid calculating layouts when nothing changed

    std::vector<Control*> children;
	std::vector<u32> childrenIndex;
	Layout::getChildrenToLayout(*container, children, &childrenIndex);

	// Get container bounds with padding
	sn::IntRect localBounds = container->getLocalClientBounds();
	localBounds.x() = 0;
	localBounds.y() = 0;
	container->getPadding().crop(localBounds);

	IntRect cellBounds;
	cellBounds.origin() = localBounds.origin();

	// TODO lastChildFill parameter?

	u32 xi = opposite(m_orientation);
	u32 yi = m_orientation;

	cellBounds.size()[xi] = localBounds.size()[xi];

	std::vector<IntRect> calculatedBounds;
	calculatedBounds.resize(children.size());

	// Space left for filler controls
	s32 fillSpace = localBounds.size()[yi];

	// Calculate child bounds
	for (u32 i = 0; i < children.size(); ++i)
	{
		Control & child = *children[i];

		const Border & margin = child.getMargin();
		IntRect childBounds = child.getLocalClientBounds();

		childBounds.origin() = cellBounds.origin();

		// Anchors
		cellBounds.size()[yi] = childBounds.size()[yi] + (m_orientation == TGUI_VERTICAL ? (margin.left + margin.right) : (margin.top + margin.bottom));
		applyAnchors(childBounds, cellBounds, child.getAnchors());

		// Margin
		child.getMargin().crop(childBounds);

		calculatedBounds[i] = childBounds;

		u32 h = cellBounds.size()[yi] + m_spacing;
		cellBounds.origin()[yi] += h;

		// If the control is not a filler
		if (m_fillers.find(childrenIndex[i]) != m_fillers.end())
		{
			// Subtracts its size from available fill space
			fillSpace -= h;
		}
	}

	// Calculate fillers
	if (!m_fillers.empty())
	{
		u32 fillSize = fillSpace < 0 ? 0 : fillSpace / m_fillers.size();
		for (auto it = m_fillers.begin(); it != m_fillers.end(); ++it)
		{
			u32 i = *it;
			IntRect & r = calculatedBounds[i];
			s32 delta = fillSize - r.size()[yi];
			if (delta < 0)
				delta = 0;
			r.size()[yi] += delta;
			// Push other controls
			for (u32 j = i + 1; j < calculatedBounds.size(); ++j)
			{
				calculatedBounds[j].origin()[yi] += delta;
			}
		}
	}

	// Apply
	for (u32 i = 0; i < calculatedBounds.size(); ++i)
	{
		const auto & p = calculatedBounds[i];
		Control & child = *children[i];
		child.setLocalClientBounds(p);
		child.layoutChildren();
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

	Variant orientationData = o["orientation"];
	if (!orientationData.isNil())
	    tgui::unserialize(orientationData, m_orientation);
}

} // namespace tgui

