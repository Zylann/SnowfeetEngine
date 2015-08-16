#ifndef __HEADER_TGUI_LAYOUT__
#define __HEADER_TGUI_LAYOUT__

#include <core/types.h>
#include <core/util/Variant.h>
#include <core/asset/SerializationContext.h>

namespace tgui
{

class Control;

/// \brief Controls how children items are positionned inside their parent
class Layout
{
public:
    Layout(Control & control) : r_control(control) {}
    virtual ~Layout() {}

    virtual void update() = 0;

    virtual void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) {};
    virtual void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) {};

protected:
    Control & r_control;

};

} // namespace tgui

#endif // __HEADER_TGUI_LAYOUT__

