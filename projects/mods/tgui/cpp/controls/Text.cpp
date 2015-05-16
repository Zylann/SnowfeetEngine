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

    batch.setScissor(controlBounds, getWindowID());

    batch.drawText(
        m_model,
        controlBounds,
        *font,
        theme->textFormat,
        m_align,
        theme->textColor
    );

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


