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

    // TODO Handle multi-line text (implement TextModel for better text processing?)
    batch.drawTextLine(
        m_sourceText.c_str(), 
        m_sourceText.size(), 
        bounds, 
        *font, 
        theme->textFormat, 
        m_align, 
        theme->textColor
   );
}

void Text::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    sn::serialize(o["text"], m_sourceText);
    tgui::serialize(o["align"], m_align);
}

void Text::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    sn::unserialize(o["text"], m_sourceText);
    tgui::unserialize(o["align"], m_align);
}

} // namespace tgui


