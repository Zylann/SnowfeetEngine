/*
Event.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_EVENT__
#define __HEADER_SN_EVENT__

#include <core/types.hpp>
#include <core/system/gui/Mouse.hpp>
#include <core/system/gui/Keyboard.hpp>
#include <cstring> // For memset

namespace sn
{

enum EventType
{
    SN_EVENT_RAW,

    SN_EVENT_WINDOW_CLOSED,
    SN_EVENT_WINDOW_MOVED,
    SN_EVENT_WINDOW_RESIZED,
    SN_EVENT_WINDOW_MINIMIZED,
    SN_EVENT_WINDOW_RESTORED,
    SN_EVENT_WINDOW_GAINED_FOCUS,
    SN_EVENT_WINDOW_LOST_FOCUS,

    SN_EVENT_KEY_DOWN,
    SN_EVENT_KEY_UP,

    SN_EVENT_TEXT_ENTERED,

    SN_EVENT_MOUSE_LEFT,
    SN_EVENT_MOUSE_MOVED,
    SN_EVENT_MOUSE_WHEEL_MOVED,
    SN_EVENT_MOUSE_DOWN,
    SN_EVENT_MOUSE_UP,
    SN_EVENT_MOUSE_ENTERED,

    SN_EVENT_JOYSTICK_CONNECTED,
    SN_EVENT_JOYSTICK_DISCONNECTED,
    SN_EVENT_JOYSTICK_DOWN, // Button down
    SN_EVENT_JOYSTICK_UP, // Button up
    SN_EVENT_JOYSTICK_MOVED, // Axis delta

    SN_EVENT_TOUCH_DOWN,
    SN_EVENT_TOUCH_UP,
    SN_EVENT_TOUCH_MOVE,

    SN_EVENT_COUNT // Keep last
};

struct SN_API Event
{
    struct WindowEvent
    {
        s32 x;
        s32 y;
        u32 width;
        u32 height;
    };

    struct KeyboardEvent
    {
        KeyCode keyCode;
        bool alt;
        bool control;
        bool shift;
    };

    struct TextEvent
    {
        u32 unicode;
    };

    struct MouseEvent
    {
        MouseButton button;
        s32 x;
        s32 y;
        s32 lastX;
        s32 lastY;
        s32 wheelDelta;
    };

    struct JoystickEvent
    {
        u32 id;
        u32 button;
        u32 axis;
        f32 axisPos;
        f32 lastAxisPos;
    };

    struct TouchEvent
    {
        u32 id;
        s32 x;
        s32 y;
        s32 lastX;
        s32 lastY;
    };

    struct RawEvent
    {
        s32 type;
        s32 wparam;
        s32 lparam;
    };

    Event(EventType type_)
    {
        memset(this, 0, sizeof(Event));
        type = type_;
    }

    EventType type;

    /// \brief ID of the window where the event occurred
    u32 windowID;

    // TODO union? (we should be able to pick the window ID on all events)
    WindowEvent window;
    KeyboardEvent keyboard;
    TextEvent text;
    MouseEvent mouse;
    JoystickEvent joystick;
    TouchEvent touch;
    RawEvent raw;

};

} // namespace sn

#endif // __HEADER_SN_EVENT__


