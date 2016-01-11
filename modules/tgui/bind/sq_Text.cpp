#include "../controls/Text.h"
#include "sq_tgui.h"
#include <core/squirrel/bind_macros.h>

namespace tgui
{
    namespace
    {
        CURRENT_CLASS(Text)

        SQRESULT setSource(HSQUIRRELVM vm)
        {
            GET_SELF();
            const SQChar * str = 0;
            sq_getstring(vm, 2, &str);
            self->setSource(str);
            return 0;
        }

        SQRESULT getSource(HSQUIRRELVM vm)
        {
            GET_SELF();
            std::string str;
            self->getSource(str);
            sq_pushstring(vm, str.c_str(), str.size());
            return 1;
        }
    }

void bindText(HSQUIRRELVM vm)
{
    sn::ScriptableObject::bindBase<Text>(vm)
        .setMethod("getSource", getSource)
        .setMethod("setSource", setSource, 2, "xs");
}

} // namespace tgui

