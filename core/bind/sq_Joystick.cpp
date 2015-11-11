#include <core/system/Joystick.h>
#include <core/squirrel/Class.h>
#include <core/squirrel/Table.h>
#include "sq_core.h"
//#include "../squirrel/bind_macros.h"

#define GET_ID() \
    u32 id = getInt(vm, 2);\
    if (id > Joystick::getMaxCount()) {\
        sq_throwerror(vm, "Joystick index is out of bounds");\
        return 0;\
    }

namespace sn
{
    using namespace squirrel;

    namespace
    {
        SQRESULT getMaxCount(HSQUIRRELVM vm)
        {
            sq_pushinteger(vm, Joystick::getMaxCount());
            return 1;
        }

        SQRESULT isConnected(HSQUIRRELVM vm)
        {
            GET_ID()
            const Joystick & j = Joystick::get(id);
            sq_pushbool(vm, j.isConnected());
            return 1;
        }

        SQRESULT getButtonCount(HSQUIRRELVM vm)
        {
            GET_ID()
            const Joystick & j = Joystick::get(id);
            sq_pushinteger(vm, j.getButtonCount());
            return 1;
        }

        SQRESULT getButton(HSQUIRRELVM vm)
        {
            GET_ID()
            const Joystick & j = Joystick::get(id);
            u32 button = getInt(vm, 3);
            sq_pushbool(vm, button < j.getButtonCount() ? j.getButton(button) : false);
            return 1;
        }

        SQRESULT getAxis(HSQUIRRELVM vm)
        {
            GET_ID()
            const Joystick & j = Joystick::get(id);
            u32 axis = getInt(vm, 3);
            sq_pushfloat(vm, j.hasAxis(axis) ? j.getAxis(axis) : 0);
            return 1;
        }

    }

    void bindJoystick(HSQUIRRELVM vm)
    {
        const char * name = "Joystick";

        Class c(vm, name);
        c.setPrivateConstructor()
            .setMethod("getMaxCount", getMaxCount, 0, "", true)
            .setMethod("isConnected", isConnected, 1, "i", true)
            .setMethod("getButtonCount", getButtonCount, 1, "i", true)
            .setMethod("getButton", getButton, 2, "ii", true)
            .setMethod("getAxis", getAxis, 2, "ii", true)
        ;

        RootTable(vm).setObject(name, c);
    }

} // namespace sn

