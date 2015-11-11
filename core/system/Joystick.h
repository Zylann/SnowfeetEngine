#ifndef __HEADER_SN_JOYSTICK__
#define __HEADER_SN_JOYSTICK__

#include <core/types.h>
#include <core/util/NonCopyable.h>
#include <vector>

namespace sn
{

class JoystickImpl;

/// \brief Virtual joystick giving access to gamepad axes and buttons.
/// Please note that a virtual joystick is actually bound to a slot rather than a real-life gamepad,
/// a different controller can be plugged while you use it.
class Joystick : public NonCopyable
{
public:
    /// \brief Gets how many joysticks are supported by the system.
    /// If 0, joystick input is not supported and you shouldn't use get().
    static u32 getMaxCount();

    /// \brief Gets a Joystick by its slot ID.
    /// \param joystickId: zero-based slot ID of the joystick. Must be lower than getMaxCount().
    /// \return Joystick object.
    static const Joystick & get(u32 joystickId);

    /// \brief Updates the state of all joysticks.
    static void updateJoysticks();

    /// \brief Tests if the controller is connected
    bool isConnected() const;

    /// \brief Gets the number of buttons on the controller
    u32 getButtonCount() const;

    /// \brief Gets the state of a button
    /// \param button: zero-based ID of the button
    /// \return true if pressed, false otherwise
    bool getButton(u32 button) const;

    /// \brief Tests if the controller has an axis
    /// \param axis: ID of the axis
    /// \return 
    bool hasAxis(u32 axis) const;

    /// \brief Gets the position of an axis.
    /// \param axis: ID of the axis (note: bidirectional axes have two IDs)
    /// \return value in -1..1
    f32 getAxis(u32 axis) const;

private:
    Joystick();
    ~Joystick();

    void update();

    void updateCaps();
    void updateState();

    void createImpl();
    void destroyImpl();

private:
    u32 m_id;
    bool m_connected;
    JoystickImpl * m_impl;

    struct State
    {
        std::vector<f32> axes;
        std::vector<bool> buttons;
    };

    struct Caps
    {
        u32 buttonCount;
        std::vector<bool> axes;

        Caps() : buttonCount(0) {}
    };

    State m_state;
    Caps m_caps;

private:
    static const u32 MAX_COUNT = 16;

    static void initializeJoysticks();
    static Joystick g_joysticks[MAX_COUNT];
    static u32 g_maxCount;

};

} // namespace sn

#endif // __HEADER_SN_JOYSTICK__

