#ifndef __HEADER_TGUI_BUTTON__
#define __HEADER_TGUI_BUTTON__

#include "Control.h"

namespace tgui
{

class Button : public Control
{
public:
    SN_OBJECT

    //--------------------------------
    // Event implementations
    //--------------------------------

    virtual void onMousePress(Event & e) override;
    virtual void onMouseRelease(Event & e) override;

    virtual void onPress() {}
    virtual void onRelease() {}

    void onDrawSelf(DrawBatch & batch) override;

    //--------------------------------
    // Overrides
    //--------------------------------

    void layoutChildren() override;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_BUTTON__

