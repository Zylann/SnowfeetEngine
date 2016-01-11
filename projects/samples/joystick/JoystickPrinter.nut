class JoystickPrinter extends Behaviour
{
    nextPrintTime = 0
    printInterval = 0.5

    function onReady() {
        entity.setUpdatable(true)
        local count = Joystick.getMaxCount();
        print("Max joystick count = " + Joystick.getMaxCount())
        for(local i = 0; i < count; ++i) {
            print("Joystick " + i + ": " + (Joystick.isConnected(i) ? "connected" : "none"))
        }
    }

    function onUpdate() {
        local time = entity.getScene().getTimeSinceStartup()
        if(time > nextPrintTime) {
            print("-----------")
            local j = 0

            // Print state of axes
            for(local axis = 0; axis < 8; ++axis) {
                print("Axis[" + axis + "]:" + Joystick.getAxis(j, axis))
            }

            // Print state of buttons
            local buttonCount = Joystick.getButtonCount(j)
            for(local button = 0; button < buttonCount; ++button) {
                print("Button[" + button + "]:" + Joystick.getButton(j, button))
            }

            nextPrintTime = time + printInterval
        }
    }
}
