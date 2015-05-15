#ifndef __HEADER_TGUI_TEXT__
#define __HEADER_TGUI_TEXT__

#include "Control.h"
#include "../TextModel.h"

namespace tgui
{

class Text : public Control
{
public:
    SN_ENTITY(tgui::Text, tgui::Control)

    void onDrawSelf(DrawBatch & batch) override;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

private:
    TextModel m_model;
    TextAlignment m_align;
};

} // namespace tgui

#endif // __HEADER_TGUI_TEXT__

