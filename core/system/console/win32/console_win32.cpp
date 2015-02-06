/*
console_win32.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../console.hpp"
#include <Windows.h>

namespace sn
{

//------------------------------------------------------------------------------
WORD toWin32Foreground(ConsoleColor cc)
{
    switch (cc)
    {
    case SN_CC_DARK_RED: return FOREGROUND_RED;
    case SN_CC_DARK_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
    case SN_CC_DARK_GREEN: return FOREGROUND_GREEN;
    case SN_CC_DARK_BLUE: return FOREGROUND_BLUE;
    case SN_CC_DARK_CYAN: return FOREGROUND_BLUE | FOREGROUND_GREEN;
    case SN_CC_DARK_MAGENTA: return FOREGROUND_BLUE | FOREGROUND_RED;

    case SN_CC_RED: return FOREGROUND_RED | FOREGROUND_INTENSITY;
    case SN_CC_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case SN_CC_GREEN: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case SN_CC_BLUE: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    case SN_CC_CYAN: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case SN_CC_MAGENTA: return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;

    case SN_CC_BLACK: return 0;
    case SN_CC_DARKGRAY: return FOREGROUND_INTENSITY;
    case SN_CC_GRAY: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    // White
    default: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }
}

//------------------------------------------------------------------------------
WORD toWin32Background(ConsoleColor cc)
{
    return toWin32Foreground(cc) << 4;
}

//------------------------------------------------------------------------------
ConsoleColor win32ForegroundToConsoleColor(WORD w)
{
    switch (w)
    {
    case FOREGROUND_RED: return SN_CC_DARK_RED;
    case FOREGROUND_RED | FOREGROUND_GREEN: return SN_CC_DARK_YELLOW;
    case FOREGROUND_GREEN: return SN_CC_DARK_GREEN;
    case FOREGROUND_BLUE: return SN_CC_DARK_BLUE;
    case FOREGROUND_BLUE | FOREGROUND_GREEN: return SN_CC_DARK_CYAN;
    case FOREGROUND_BLUE | FOREGROUND_RED: return SN_CC_DARK_MAGENTA;

    case FOREGROUND_RED | FOREGROUND_INTENSITY: return SN_CC_RED;
    case FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY: return SN_CC_YELLOW;
    case FOREGROUND_GREEN | FOREGROUND_INTENSITY: return SN_CC_GREEN;
    case FOREGROUND_BLUE | FOREGROUND_INTENSITY: return SN_CC_BLUE;
    case FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY: return SN_CC_CYAN;
    case FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY: return SN_CC_MAGENTA;

    case 0: return SN_CC_BLACK;
    case FOREGROUND_INTENSITY: return SN_CC_DARKGRAY;
    case FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE: SN_CC_GRAY;

    // White
    default: return SN_CC_WHITE;
    }
}

//------------------------------------------------------------------------------
ConsoleColor win32BackgroundToConsoleColor(WORD w)
{
    return win32ForegroundToConsoleColor(w >> 4);
}

//------------------------------------------------------------------------------
ConsoleColor getConsoleForeground()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    WORD wColorAttributes = csbiInfo.wAttributes;
    return win32ForegroundToConsoleColor(wColorAttributes);
}

//------------------------------------------------------------------------------
ConsoleColor getConsoleBackground()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    WORD wColorAttributes = csbiInfo.wAttributes;
    return win32BackgroundToConsoleColor(wColorAttributes);
}

//------------------------------------------------------------------------------
void setConsoleForeground(ConsoleColor color)
{
    if (color == SN_CC_DEFAULT)
        color = SN_CC_GRAY;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, toWin32Foreground(color));
}

//------------------------------------------------------------------------------
void setConsoleBackground(ConsoleColor color)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, toWin32Background(color));
}

} // namespace sn

