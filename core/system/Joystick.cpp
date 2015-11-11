#include "Joystick.h"
#include <core/util/assert.h>

namespace sn
{

//-----------------------------------------------------------------------------
Joystick Joystick::g_joysticks[Joystick::MAX_COUNT];
u32 Joystick::g_maxCount = 0;

//-----------------------------------------------------------------------------
void Joystick::initializeJoysticks()
{
    g_maxCount = getMaxCount();
    if (g_maxCount)
    {
        for (u32 i = 0; i < g_maxCount; ++i)
        {
            Joystick & j = g_joysticks[i];
            j.m_id = i;
            j.createImpl();
        }
    }
}

//-----------------------------------------------------------------------------
void Joystick::updateJoysticks()
{
    for (u32 i = 0; i < g_maxCount; ++i)
    {
        Joystick & j = g_joysticks[i];
        j.update();
    }
}

//-----------------------------------------------------------------------------
const Joystick & Joystick::get(u32 joystickId)
{
    static bool s_initialized = false;
    if (!s_initialized)
    {
        initializeJoysticks();
        updateJoysticks();
        s_initialized = true;
    }
    SN_ASSERT(joystickId < g_maxCount, "Joystick ID is out of bounds");
    return g_joysticks[joystickId];
}

//-----------------------------------------------------------------------------
Joystick::Joystick() : 
    m_id(-1), 
    m_connected(false),
    m_impl(nullptr)
{
}

//-----------------------------------------------------------------------------
Joystick::~Joystick()
{
    destroyImpl();
}

//-----------------------------------------------------------------------------
u32 Joystick::getButtonCount() const
{
    if (m_connected)
        return m_caps.buttonCount;
    return 0;
}

//-----------------------------------------------------------------------------
void Joystick::update()
{
    if (isConnected())
    {
        if (!m_connected)
        {
            updateCaps();
            m_connected = true;
        }

        updateState();
    }
    else
    {
        m_connected = false;
    }
}

//-----------------------------------------------------------------------------
bool Joystick::hasAxis(u32 axis) const
{
    if (m_connected)
        return axis < m_caps.axes.size() && m_caps.axes[axis];
    return false;
}

//-----------------------------------------------------------------------------
float Joystick::getAxis(u32 axis) const
{
    if (m_connected)
        return m_state.axes[axis];
    return false;
}

//-----------------------------------------------------------------------------
bool Joystick::getButton(u32 button) const
{
    if (m_connected)
        return m_state.buttons[button];
    return false;
}


} // namespace sn

