#ifndef __HEADER_TGUI_TEXT__
#define __HEADER_TGUI_TEXT__

#include <modules/tgui/controls/Control.h>
#include <modules/tgui/TextModel.h>
#include <modules/tgui/TextAlignment.h>

namespace tgui
{

/// \brief Lightweight text displayer.
class SN_TGUI_API Text : public Control
{
public:
    SN_OBJECT

    Text();

    void onDrawSelf(DrawBatch & batch) override;

    void setSource(const std::string & str);
    void getSource(std::string & out);

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

private:
    TextModel m_model;
    TextAlignment m_align;
};

} // namespace tgui

#endif // __HEADER_TGUI_TEXT__

