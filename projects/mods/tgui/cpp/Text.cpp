#include "Text.hpp"
#include "Theme.hpp"

using namespace sn;

namespace tgui
{

void Text::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    Font * font = theme->getFont();
    if (font == nullptr)
        return;

    IntRect bounds = getClientBounds();
    batch.drawText(m_sourceText, bounds.origin(), *font, theme->textFormat, theme->textColor);
}

void Text::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    sn::serialize(o["text"], m_sourceText);
}

void Text::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    sn::unserialize(o["text"], m_sourceText);
}

} // namespace tgui


