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
    IntRect controlBounds = getClientBounds();

    // TODO Handle multi-line text (implement TextModel for better text processing?)
    IntRect bounds = controlBounds;
    s32 lineHeight = font->getLineHeight(format.size);

    batch.setScissor(bounds, getWindowID());

    for (u32 i = 0; i < m_model.getLineCount(); ++i)
    {
        const std::string & line = m_model.getLine(i);

        batch.drawTextLine(
            line.c_str(), 
            line.size(), 
            bounds, 
            *font, 
            theme->textFormat, 
            m_align, 
            theme->textColor
        );

        if (lineHeight > bounds.height())
            break;
        bounds.y() += lineHeight;
        bounds.height() -= lineHeight;
    }

    batch.disableScissor();
}

void Text::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    
    std::string text;
    m_model.getSource(text);
    sn::serialize(o["text"], text);

    tgui::serialize(o["align"], m_align);
}

void Text::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

    std::string text;
    sn::unserialize(o["text"], text);
    m_model.setSource(text);

    tgui::unserialize(o["align"], m_align);
}

} // namespace tgui


