#include <core/math/math.h>
#include "../Joystick.h"
#include "Joystick_win32.h"
#include <Windows.h>

namespace sn
{

//-----------------------------------------------------------------------------
    class JoystickImpl
    {
    public:
        JOYCAPS caps;
    };

//-----------------------------------------------------------------------------
    namespace
    {
        enum WinmmJoyAxes
        {
            AXIS_X = 0,
            AXIS_Y,
            AXIS_Z,
            AXIS_R,
            AXIS_U,
            AXIS_V,
            AXIS_POVX,
            AXIS_POVY,
            AXIS_COUNT
        };
    }

//-----------------------------------------------------------------------------
void Joystick::createImpl()
{
    m_impl = new JoystickImpl();
}

//-----------------------------------------------------------------------------
void Joystick::destroyImpl()
{
    if (m_impl)
    {
        delete m_impl;
        m_impl = nullptr;
    }
}

//-----------------------------------------------------------------------------
u32 Joystick::getMaxCount()
{
    u32 max = joyGetNumDevs();
    if (max > Joystick::MAX_COUNT)
        max = Joystick::MAX_COUNT;
    return max;
}

//-----------------------------------------------------------------------------
bool Joystick::isConnected() const
{
    JOYINFO info;
    MMRESULT res = joyGetPos(m_id, &info);
    return res == JOYERR_NOERROR;
}

//-----------------------------------------------------------------------------
void Joystick::updateCaps()
{
    JOYCAPS & caps = m_impl->caps;
    MMRESULT res = joyGetDevCaps(m_id, &caps, sizeof(caps));
    if (res != JOYERR_NOERROR)
        return;

    m_caps.buttonCount = caps.wNumButtons;
    
    m_caps.axes.resize(AXIS_COUNT, false);

    m_caps.axes[AXIS_X] = true;
    m_caps.axes[AXIS_Y] = true;
    m_caps.axes[AXIS_Z] = (caps.wCaps & JOYCAPS_HASZ);
    m_caps.axes[AXIS_R] = (caps.wCaps & JOYCAPS_HASR);
    m_caps.axes[AXIS_U] = (caps.wCaps & JOYCAPS_HASU);
    m_caps.axes[AXIS_V] = (caps.wCaps & JOYCAPS_HASV);

    m_state.axes.resize(m_caps.axes.size());
    m_state.buttons.resize(m_caps.buttonCount);
}

//-----------------------------------------------------------------------------
namespace
{
    inline f32 normalizedAxis(DWORD x, UINT min, UINT max)
    {
        return (2.f * x - (min + max)) / (max - min);
    }
}

//-----------------------------------------------------------------------------
void Joystick::updateState()
{
    JOYINFOEX info;
    info.dwSize = sizeof(JOYINFOEX);
    info.dwFlags = JOY_RETURNBUTTONS 
                    | JOY_RETURNR 
                    | JOY_RETURNU 
                    | JOY_RETURNV 
                    | JOY_RETURNX 
                    | JOY_RETURNY 
                    | JOY_RETURNZ;

    const JOYCAPS & caps = m_impl->caps;

    info.dwFlags |= (info.dwFlags & JOYCAPS_POVCTS) ? JOY_RETURNPOVCTS : JOY_RETURNPOV;

    if (joyGetPosEx(m_id, &info) == JOYERR_NOERROR)
    {
        m_state.axes[AXIS_X] = normalizedAxis( info.dwXpos, caps.wXmin, caps.wXmax );
        m_state.axes[AXIS_Y] = normalizedAxis( info.dwYpos, caps.wYmin, caps.wYmax );
        m_state.axes[AXIS_Z] = normalizedAxis( info.dwZpos, caps.wZmin, caps.wZmax );
        m_state.axes[AXIS_R] = normalizedAxis( info.dwRpos, caps.wRmin, caps.wRmax );
        m_state.axes[AXIS_U] = normalizedAxis( info.dwUpos, caps.wUmin, caps.wUmax );
        m_state.axes[AXIS_V] = normalizedAxis( info.dwVpos, caps.wVmin, caps.wVmax );

        if (info.dwPOV != 0xfff)
        {
            f32 angle = math::DEG2RAD * info.dwPOV / 100.f;
            m_state.axes[AXIS_POVX] = cos(angle);
            m_state.axes[AXIS_POVY] = sin(angle);
        }
        else
        {
            m_state.axes[AXIS_POVX] = 0;
            m_state.axes[AXIS_POVY] = 0;
        }

        for (u32 i = 0; i < m_caps.buttonCount; ++i)
            m_state.buttons[i] = info.dwButtons & (1 << i);
    }
    else
    {
        m_connected = false;
    }
}

} // namespace sn

