#include "../controls/Control.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

using namespace sn;

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Control)

        SQRESULT isPressed(HSQUIRRELVM vm)
        {
            GET_SELF();
            sq_pushbool(vm, self->isPressed());
            return 1;
        }

        SQRESULT setBounds(HSQUIRRELVM vm)
        {
            GET_SELF();
            sn::IntRect bounds = sn::IntRect::fromPositionSize(
                squirrel::getInt(vm, 2),
                squirrel::getInt(vm, 3),
                squirrel::getInt(vm, 4),
                squirrel::getInt(vm, 5)
            );
            self->setLocalBounds(bounds);
            return 0;
        }

        SQRESULT setAnchors(HSQUIRRELVM vm)
        {
            GET_SELF();
            Anchors anchors;
            for (u32 i = 0; i < anchors.size(); ++i)
            {
                anchors.set(i, squirrel::getBool(vm, 2 + i));
            }
            self->setAnchors(anchors);
            return 0;
        }
    }

void bindControl(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Control>(vm)
        .setMethod("isPressed", isPressed)
        .setMethod("setBounds", setBounds, 5, "xiiii")
        .setMethod("setAnchors", setAnchors, 5, "xbbbb")
    ;
}

} // namespace tgui

