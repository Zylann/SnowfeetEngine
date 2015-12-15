#ifndef __HEADER_TGUI_TEXT__
#define __HEADER_TGUI_TEXT__

#include "Control.h"
#include "../TextModel.h"
#include "../TextAlignment.h"

namespace tgui
{

/// \brief Lightweight text displayer.
class Text : public Control
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

