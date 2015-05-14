#include "Text.h"
#include "../theme/Theme.h"

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

    const FontFormat & format = theme->textFormat;

    IntRect bounds = getClientBounds();
    Vector2i offset(0, font->getLineHeight(format.size));
    batch.drawText(m_sourceText, bounds.origin() + offset, *font, theme->textFormat, theme->textColor);
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


