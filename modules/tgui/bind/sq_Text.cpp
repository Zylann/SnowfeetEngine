#include "../controls/Text.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Text)

        BEGIN_METHOD(set)
            const SQChar * str = 0;
            sq_getstring(vm, 2, &str);
            self->setSource(str);
            return 0;
        END_METHOD

        BEGIN_METHOD(get)
            std::string str;
            self->getSource(str);
            sq_pushstring(vm, str.c_str(), str.size());
            return 1;
        END_METHOD
    }

void bindText(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Text>(vm)
        .setMethod("getSource", get)
        .setMethod("setSource", set, 1, "s");
}

} // namespace tgui

