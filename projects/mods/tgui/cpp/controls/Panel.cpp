#include "Panel.hpp"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
void Panel::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;

    const ControlTheme & ct = theme->panelTheme;

    IntRect bounds = getClientBounds();

    const IntRect & uvRect = ct.statesUV[0];
    Vector2u ts = theme->getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);
}

//------------------------------------------------------------------------------
void Panel::onMousePress(Event & e)
{
    e.consume();
}

//------------------------------------------------------------------------------
void Panel::onMouseRelease(Event & e)
{
    e.consume();
}

} // namespace tgui


